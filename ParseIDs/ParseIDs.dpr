program ParseIDs;

{$APPTYPE CONSOLE}

{$R *.res}

uses
    System.SysUtils
  , System.Classes
  , System.IOUtils
  ;

type
  TALrecord = record
    ID:     int64;
    Offset: int64;
  end;
  TADrecord = record
    ID: int64;
    Offset: int64;
    Rel:  int64;
    IDC:  string;
  end;
  TADfile = file of int64;
  TADrecords = array of array of TADrecord;

const
  prologue = '#include <idc.idc>'+#10+#13+#10+#13+
    'static MySetName ( ea , name, flags )'+#10+#13+'{'+#10+#13+
    #9+'auto currCmt;'+#10+#13+
    #9+'auto currName;'+#10+#13+
    #9+'auto currFlags;'+#10+#13+
    #10+#13+
    #9+'if ( ( currName = get_name(ea) ) == "" )'+#10+#13+
        #9+#9+'set_name ( ea, name, flags );'+#10+#13+
    #9+'else if ( currName == name ) { }'+#10+#13+
    #9+'else if ( substr(currName, 0, 5) == "__ICI" )'+#10+#13+
        #9+#9+'set_name ( ea, name, flags );'+#10+#13+
    #9+'else'+#10+#13+
    #9+'{'+#10+#13+
    #9+#9+'currFlags = get_flags(ea);'+#10+#13+
    #9+#9+'if ( ( currFlags & SN_AUTO ) != 0 )'+#10+#13+
        #9+#9+#9+'set_name ( ea, name, flags );'+#10+#13+
    #9+#9+'else if ( ( currCmt = get_cmt ( ea, 0 ) ) == "" )'+#10+#13+
        #9+#9+#9+'set_cmt ( ea, name, 0 );'+#10+#13+
    #9+#9+'else if ( currCmt == name ) { }'+#10+#13+
    #9+#9+'else'+#10+#13+
        #9+#9+#9+'set_cmt ( ea, currCmt + " " + name, 0 ) ;'+#10+#13+
    #9+'}'+#10+#13+#10+#13+
    '}'+#10+#13+#10+#13+
    'static main(void)'+#10+#13+'{';
  epilogue = '}'+#10+#13+#10+#13;
  prologueCsv = 'Version;Index;ID;Offset;Relocation;HexRelocation;IDC;Comment';
  epilogueCsv = '';

var
  SourceDir    : string = 'D:\VMods\fallout4\mods\Address Library - All In One-47327-AIO-1715667241\F4SE\Plugins';
  SourceVer    : string = '*';
  BaseModule   : int64 = $140000000 ;
  CountPerFile : int64 = 10000000 ;
  adVersions   : TStringList;
  IDsFile      : string = 'C:\Local\F4\DEV\GitHubWork\libxse\commonlibf4\include\RE\IDs.h';
  namespace    : string = '';
  lowerLetters : TSysCharSet = ['a'..'z','0'..'9','_'];
  AllChars     : TSysCharSet = ['a'..'z','0'..'9','_','{','}',':'];
  Idents       : TStringList;
  IDCs         : TStringList;

  function OpenNamespace(namespace : string; Terms : TStringList; var i : integer; var l : string) : string;
  begin
    i := i + 1;
    if i < Terms.Count then
    begin
      l := Terms[i];
      if Length(namespace)>0 then namespace := namespace + '::';
      namespace := namespace + l;
    end;
    Result := namespace;
  end;

  function CloseNamespace(namespace : string): string;
  begin
    var n : string := namespace;
    var j : integer;
    Result := '';
    repeat
      j := Pos('::', n);
      if j >0 then
      begin
        Result := Result + Copy(n, 1, j+1);
        Delete(n, 1, j+1);
      end;
    until j=0;
    if Length(Result)>1 then Delete(Result, Length(Result)-1, 2);
  end;

procedure DoParseIDs;
var
  IDs     : TStringList;
  Terms   : TStringList;
  ident   : string;
  i       : integer;
  l       : string;
  j       : integer;
  InIdent : boolean;
  ID      : int64;

begin
  if not Assigned(Idents)  then Exit;

  try
    Terms := TStringList.Create;
    try
      IDs := TStringList.Create;
      IDs.LoadFromFile(IDsFile);
      for i := 0 to IDs.Count - 1 do
      begin
        if (i mod 1000)=0 then Write('*');
        l := IDs[i];
        j := Pos('//', l); if j>0 then Delete(l, j, length(l));
        j := Pos('#', l); if j>0 then Delete(l, j, length(l));
        repeat
          while (Length(l)>0) and not (CharInSet(LowerCase(l)[1], AllChars)) do
            Delete(l, 1, 1);
          j := 0;
          while (Length(l)>j) and (CharInSet(LowerCase(l)[j+1], lowerLetters+[':'])) do
            Inc(j);
          if j = 0 then
            while (Length(l)>j) and (CharInSet(LowerCase(l)[j+1], ['{','}'])) do
              Inc(j);
          if j > 0 then
          begin
            if j = Length(l) then
              Terms.Add(l)
            else
              Terms.Add(Copy(l, 1, j));
            Delete(l, 1, j);
          end;
        until Length(l) = 0;
      end;
    finally
      FreeAndNil(IDs);
    end;
    Terms.SaveToFile(IDsFile+'.debug.Terms');
    Ident := '';
    InIdent := False;
    i := 0;
    while i < Terms.Count do
    begin
      if (i mod 1000)=0 then Write('+');
      l := Terms[i];
      if SameText(l, 'namespace') then
        namespace := OpenNamespace(namespace, Terms, i, l)
      else if SameText('inline', l) or SameText('constexpr', l) or SameText('REL::ID', l) then
        InIdent := True
      else if SameText('}', l) then
        if InIdent then
          InIdent := False
        else
          namespace := CloseNamespace(namespace)
      else
        if SameText('{', l) then
        begin
          if InIdent then
          begin
            Inc(i);
            if i < Terms.Count then
            begin
              ID := StrToInt(Terms[i]);
              Idents.AddObject(Namespace+'::'+Ident, TObject(ID));
            end;
          end;
        end
      else
        Ident := l;
      Inc(i);
    end;
    Idents.SaveToFile(IDsFile+'.idents');
    WriteLN;
  finally
    FreeAndNil(Terms);
  end;
end;

function MakeIDC(name: string; ID: int64; Offset: int64; Rel : int64; j : integer; IDasText : string): string;
begin
  Result := '      MySetName( 0x0' + IntToHex(baseModule) + ' + 0x0' + IntToHex(Offset) +
              ', "ADF4::' + name + '", SN_FORCE + SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // ' +
              IntToHex(Offset,0) + ' ' + IntToHex(Rel,0) + ' 0x0' + IntToHex(Rel,0) + ' Index:' + IntToStr(j) + IDasText;
end;

function MakeComment(name: string; ID: int64): string;
begin
  Result := '  //  MySetName( 0x0' + IntToHex(baseModule) + ' + 0x0' + IntToHex(0) +
              ', "ADF4::' + name + '", SN_FORCE + SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // ' +
              IntToHex(0,0) + ' ' + IntToHex(0,0) + ' 0x0' + IntToHex(0,0) + ' Index:' + IntToStr(0) + #9 + 'ID:' + IntToStr(ID);
end;

begin
  try
    { TODO -oUser -cConsole Main : Insert code here }
    var i : integer := 1;
    while i <= ParamCount do
    begin
      var param : string := ParamStr(i);
      if (Length(param)>2) and (param[1]='-') and (param[3]='=') then
      case Ord(UpperCase(param)[2]) of
        Ord('P'): SourceDir := Trim(Copy(param,4,Length(param)));
        Ord('V'): SourceVer := Trim(Copy(param,4,Length(param)));
        Ord('B'): BaseModule := StrToInt(Trim(Copy(param,4,Length(param))));
        Ord('C'): CountPerFile := StrToInt(Trim(Copy(param,4,Length(param))));
        Ord('I'): IDsFile := Trim(Copy(param,4,Length(param)));
      end;
      Inc(i);
    end;
    try
      adVersions := TStringList.Create;
      var F : TSearchRec;
      var R : integer := FindFirst(SourceDir+'\'+SourceVer+'.bin', faAnyfile, F);
      while R = 0 do
      begin
        if (F.Attr and faDirectory) <> faDirectory then
          adVersions.Add(F.Name);
        R := FindNext(F);
      end;
      var fad : TADfile;
      var far : TADrecords;
      SetLength(far, adVersions.Count);
      i := 0;
      var version : string;
      for version in adVersions do
      try
        WriteLN(Version);
        AssignFile(fad, SourceDir+'\'+version);
        Reset(fad);
        var j : integer := 0;
        var v : int64;
        var adr : TALrecord;
        Read(fad, v); // First record is count of records, 0
        var s : int64 := (FileSize(fad)-1) div 2;
        WriteLN(#9#9+' Expected Count='+IntToStr(s));
        SetLength(far[i], s);
        repeat
          Read(fad, adr.ID);
          Read(fad, adr.Offset);
          with far[i, j] do
          begin
            ID := adr.ID;
            Offset := adr.offset;
            Rel := baseModule + adr.offset;
            IDC := MakeIDC('ID'+IntToStr(ID), ID, Offset, Rel, j, '');
              //'MySetName( 0x0' + IntToHex(baseModule) + ' + 0x0' + IntToHex(adr.Offset) +
              //', "ADF4::ID' + IntToStr(adr.ID) + '", SN_FORCE + SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // ' +
              //IntToHex(Offset,0) + ' ' + IntToHex(Rel,0) + ' 0x0' + IntToHex(Rel,0) + ' Index:' + IntToStr(j);
          end;
          if (j mod 1000)=0 then Write('.');
          Inc(j);
        until Eof(fad);
        CloseFile(fad);
        WriteLN;
        WriteLN(#9#9+'Count='+IntToStr(j));
        WriteLN;
        Inc(i);
      finally

      end;

      try
        Idents := TStringList.Create;
        DoParseIDs;

        var fcsv : Text;
        AssignFile(fcsv, SourceDir + '\AllVersions.csv');
        Rewrite(fcsv);
        WriteLN(fcsv, prologueCsv);
        if not DirectoryExists(SourceDir + '\idc') then TDirectory.CreateDirectory(SourceDir + '\idc');

        try
          IDCs := TStringList.Create;
          try
            var fi : Text;
            for i := 0 to Length(far)-1 do
              try
                version := ChangeFileExt(adVersions[i], '.idc');
                var ThisVersion : Boolean;
                ThisVersion := not SameText(SourceVer, '*') and SameText(version, SourceVer+'.idc');

                WriteLN(Version);
                WriteLN(#9#9+'Count='+IntToStr(Length(far[i])));
                WriteLN;
                try
                  if not DirectoryExists(SourceDir+'\idc\'+adVersions[i]) then TDirectory.CreateDirectory(SourceDir+'\idc\'+adVersions[i]);
                  AssignFile(fi, SourceDir+'\idc\'+adVersions[i]+'\'+version);
                  Rewrite(fi);
                  WriteLN(fi, Prologue);
                  var j : integer;
                  for j := 0 to Length(far[i])-1 do
                    begin
                      WriteLN(fi, #9+far[i, j].IDC);
                      WriteLN(fcsv, adVersions[i] + ';' + IntToStr(far[i, j].ID) + ';' + IntToHex(far[i, j].Offset,0) + ';' +
                        IntToHex(far[i, j].Rel,0) + ';' + '0x0'+IntToHex(far[i, j].Rel,0) + ';' + far[i, j].IDC );
                      if (j mod 1000)=0 then Write('.');
                      if (j>0) and ((j mod CountPerFile)=0) then
                      begin
                        WriteLN;
                        WriteLN(fi, epilogue);
                        Close(fi);
                        version := ExtractFileName(adVersions[i]);
                        version := ChangeFileExt(version, '')+'-'+IntToStr(j);
                        version := ChangeFileExt(version, '.idc');
                        WriteLN(#9+Version);
                        WriteLN;
                        AssignFile(fi, SourceDir+'\idc\'+adVersions[i]+'\'+version);
                        Rewrite(fi);
                        WriteLN(fi, Prologue);
                      end;
                      if ThisVersion then
                      begin
                        var k : integer;
                        for k := 0 to Idents.Count - 1 do
                        begin
                          if Int64(Idents.Objects[k])=far[i, j].ID then
                          begin
                            IDCs.Add(MakeIDC(
                              Idents[k],
                              far[i, j].ID,
                              far[i,j].Offset,
                              far[i,j].Rel,
                              j,
                              #9+'ID:'+IntToStr(far[i,j].ID)));
                            Idents.Objects[k] := TObject(0);
                          end;
                        end;
                      end;
                    end;
                finally
                  WriteLN(fi, epilogue);
                  Close(fi);
                  WriteLN;
                end;
              except
              end;
          finally
            WriteLN(fcsv, epilogueCsv);
            Close(fcsv);
          end;
        finally
          if IDCs.Count>0 then begin
            var HasNotFound := false;
            for i:= 0 to Idents.Count - 1 do
              if Idents.Objects[i] <> TObject(0) then
              begin
                if not HasNotFound then
                begin
                  HasNotFound := True;
                  IDCs.Add(#10+#13+'// ID not found!'+#10+#13+#10+#13);
                end;
                IDCs.Add(MakeComment(Idents[i], Int64(Idents.Objects[i])));
              end;
            IDCs.Insert(0, prologue);
            IDCs.Add(epilogue);
            IDCs.SaveToFile(SourceDir + '\'+SourceVer+'.idc');
            IDCs.Clear;
          end;
          FreeAndNil(IDCs);
        end;
      finally
        FreeAndNil(Idents);
      end;
    finally
      FreeAndNil(adVersions);
    end;
  except
    on E: Exception do
      Writeln(E.ClassName, ': ', E.Message);
  end;
end.
