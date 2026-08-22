program ParseIDsSF;

{$APPTYPE CONSOLE}

{$R *.res}

uses
  System.SysUtils,
  System.Classes,
  System.IOUtils;

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
  TADfile = TFileStream;
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
    #9+'else if ( currName == name ) { msg("#"); }'+#10+#13+
    #9+'else if ( substr(currName, 0, 5) == "__ICI" )'+#10+#13+
        #9+#9+'set_name ( ea, name, flags );'+#10+#13+
    #9+'else if ( !has_user_name(get_flags(ea)) )'+#10+#13+
    #9+#9+#9+'set_name ( ea, name, flags );'+#10+#13+
    #9+'else {'+#10+#13+
    #9+#9+'msg(".");'+#10+#13+
    #9+#9+'if ( ( currCmt = get_cmt ( ea, 0 ) ) == "" )'+#10+#13+
        #9+#9+#9+'set_cmt ( ea, name, 0 );'+#10+#13+
    #9+#9+'else if ( currCmt == name ) { msg("#"); }'+#10+#13+
    #9+#9+'else'+#10+#13+
        #9+#9+#9+'set_cmt ( ea, currCmt + " " + name, 0 ) ;'+#10+#13+
    #9+'}'+#10+#13+#10+#13+
    '}'+#10+#13+#10+#13+
    'static main(void)'+#10+#13+'{'+#10+#13+
    #9+'msg("Start\n");'+#10+#13+
    #9+'auto_wait();'+#10+#13+
    #9+'msg("Run\n");'+#10+#13+
    #10+#13+#10+#13;
  epilogue = #10+#13+#10+#13+
    #9+'msg("\nDone\n");'+#10+#13+#10+#13+
    '}'+#10+#13+#10+#13;
  prologueCsv = 'Version;Index;ID;Offset;Relocation;HexRelocation;IDC;Comment';
  epilogueCsv = '';

var
  SourceDir : string = 'C:\Local\F4\DEV\CurrentAL';
  SourceVer : string = '*';
  BaseModule : int64 = $140000000 ;
  CountPerFile : int64 = 10000000 ;
  adVersions   : TStringList;
  IDsFile      : string = 'C:\Local\F4\DEV\GitHubWork\libxse\commonlibf4\include\RE\IDs.h';
  namespace    : string = '';
  namespaceT   : string = '';
  lowerLetters : TSysCharSet = ['a'..'z','0'..'9','_'];
  AllChars     : TSysCharSet = ['a'..'z','0'..'9','_','{','}',':'];
  Idents       : TStringList;
  IDCs         : TStringList;

(*
Source - https://stackoverflow.com/a
Posted by Ramon
Retrieved 2025-11-27, License - CC BY-SA 3.0
*)

function IsOpen(const txt:TextFile):Boolean;
const
  fmTextOpenRead = 55217;
  fmTextOpenWrite = 55218;
begin
  Result := (TTextRec(txt).Mode = fmTextOpenRead) or (TTextRec(txt).Mode = fmTextOpenWrite)
end;

function OpenNamespace(namespace : string; Terms : TStringList; var i : integer; var l : string) : string;
begin
  i := i + 1;
  if i < Terms.Count then
  begin
    l := Terms[i];
    if Length(namespace)>0 then namespace := namespace + '::::';
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
    j := Pos('::::', n);
    if j >0 then
    begin
      Result := Result + Copy(n, 1, j+1);
      Delete(n, 1, j+1);
    end;
  until j=0;
  if Length(Result)>1 then Delete(Result, Length(Result)-1, 2);
end;

function PrepNamespace(namespace : string): string;
begin
  var s : string := namespace;
  var p : integer;
  repeat
    p := Pos('::::', s);
    if p > 0 then s := Copy(s, 1, p) + Copy(s, p+3, length(s));
  until p = 0;
  Result := s;
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
              namespaceT := PrepNamespace(namespace);
              Idents.AddObject(NamespaceT+'::'+Ident, TObject(ID));
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

function MakeName(name: string; ID: int64; Offset: int64; Rel : int64; j : integer; IDasText : string): string;
begin
  Result := '      MySetName( 0x0' + IntToHex(baseModule) + ' + 0x0' + IntToHex(Offset) +
              ', "ADSF::' + name + '", SN_FORCE + SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // ' +
              IntToHex(Offset,0) + ' ' + IntToHex(Rel,0) + ' 0x0' + IntToHex(Rel,0) + ' Index:' + IntToStr(j) + IDasText;
end;

function MakeComment(name: string; j : integer; ID: int64): string;
begin
  Result := '  //  MySetName( 0x0' + IntToHex(baseModule) + ' + 0x0' + IntToHex(0) +
              ', "ADSF::' + name + '", SN_FORCE + SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // ' +
              IntToHex(0,0) + ' ' + IntToHex(0,0) + ' 0x0' + IntToHex(0,0) + ' Index:' + IntToStr(j) + #9 + 'ID:' + IntToStr(ID);
end;

function MakeCsv(adVersion : TStringList; i, j: integer; far: TADrecords ): string;
begin
  Result := adVersions[i] + ';' + IntToStr(j) + ';' + IntToStr(far[i, j].ID) + ';' + IntToHex(far[i, j].Offset,0) + ';' +
    IntToHex(far[i, j].Rel,0) + ';' + '0x0'+IntToHex(far[i, j].Rel,0) + ';' + far[i, j].IDC;
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
        var t : string := SourceDir+'\'+version;
        fad := TFileStream.Create(t, fmOpenRead  or fmShareDenyWrite);
        var j : integer;
        var v : int32;
        var vv: Int64;
        var h : record
          version : array [0..3] of int32;
          name : array[0..63] of Byte;
          pointerSize : int32;
          dataFormat  : int32;
          offsetCount : int32;
        end;
        var k : integer;
        var l : integer;
        var s : integer;
        var b : uint8;
        var d : uint16;
        var q : uint32;
        var ps : uint32;
        var adr : TALrecord;
        fad.Read(s, sizeof(int32)); // First record is count of records or version if 2 or 5
        if s = 2 then begin
          k := sizeof(int32);
          for j := 0 to 3 do fad.Read(h.version[j], k);  // Game version
          fad.Read(l, k); // game name length
          for j := 0 to Pred(l) do fad.Read(h.name[j], 1);  // name
          fad.Read(ps, k); // Pointer size
          fad.Read(v, k); // address count
          vv := v;
          k := 0;
        end
        else if s = 5 then begin
          k := sizeof(h) + sizeof(int32);
          Fad.ReadData(h);
          vv := h.offsetCount;
          k := (fad.Size - k) div sizeof(Int32);
        end else begin
          k := sizeof(int64);
          FreeAndNil(fad);
          fad := TFileStream.Create(t, fmOpenRead);
          fad.Read(vv, sizeof(int64));
          k := (fad.Size - k) div sizeof(Int64)*2;
        end;
        j := 0;

        WriteLN(#9#9+' Expected Count='+IntToStr(k) + ' for ' + IntToStr(vv) + ' generation ' + IntToStr(s));
        SetLength(far[i], vv);
        adr.ID := 0;
        var prevID     : uint64 := 0;
        var prevOffset : uint64 := 0;
        if s = 2 then try repeat
          v := 0;
          fad.Read(b, 1);
          var lo : integer := b and 15;
          var hi : integer := b shr 4;
          case lo of
            0: begin fad.Read(prevID, sizeof(uint64)); adr.ID := prevID; end;
            1: adr.ID := prevID + 1;
            2: begin fad.Read(b, 1); adr.ID := prevID + b; end;
            3: begin fad.Read(b, 1); adr.ID := prevID - b; end;
            4: begin fad.Read(d, 2); adr.ID := prevID + d; end;
            5: begin fad.Read(d, 2); adr.ID := prevID - d; end;
            6: begin fad.Read(d, sizeof(uint16)); adr.ID := d; end;
            7: begin fad.Read(q, sizeof(uint32)); adr.ID := q; end;
            else begin
              WriteLN(#13#13+'  Error lo type is wrong b:' + IntToStr(b) + ' hi=' + IntToStr(hi) + ' lo=' + IntToStr(lo) + ' on line ' + IntToStr(j));
              v := -1;
              break;
            end;
          end;

          var tmp : uint64 := prevOffset;
          if (hi and 8) <> 0 then tmp := prevOffset div ps;
          case hi and 7 of
            0: begin fad.Read(tmp, sizeof(uint64)); adr.Offset := tmp; end;
            1: adr.Offset := tmp + 1;
            2: begin fad.Read(b, 1); adr.Offset := tmp + b; end;
            3: begin fad.Read(b, 1); adr.Offset := tmp - b; end;
            4: begin fad.Read(d, 2); adr.Offset := tmp + d; end;
            5: begin fad.Read(d, 2); adr.Offset := tmp - d; end;
            6: begin fad.Read(d, sizeof(uint16)); adr.Offset := d; end;
            7: begin fad.Read(q, sizeof(uint32)); adr.Offset := q; end;
            else begin
              WriteLN(#13#13+'  Error hi type is wrong b:' + IntToStr(b) + ' hi=' + IntToStr(hi) + ' lo=' + IntToStr(lo) + ' on line ' + IntToStr(j));
              v := -2;
              break;
            end;
          end;
          if v >= 0 then begin
            if (hi and 8) <> 0 then adr.Offset := adr.Offset * ps;
            prevID := adr.ID;
            prevOffset := adr.Offset;
            with far[i, j] do
            begin
              ID := adr.ID;
              Offset := adr.offset;
              Rel := baseModule + adr.offset;
              IDC := MakeName('ID'+IntToStr(ID), ID, Offset, Rel, j, '');
            end;
          end;
          if (j mod 1000)=0 then Write('.');
          Inc(j);
        until j >= vv except end else if s = 5 then
        repeat
          var offset : uint32 := 0;
          fad.Read(Offset, sizeof(int32));
          adr.Offset := Offset;
          with far[i, j] do
            if adr.Offset > 0 then
              begin
                ID := adr.ID;
                Offset := adr.offset;
                Rel := baseModule + adr.offset;
                IDC := MakeName('ID'+IntToStr(ID), ID, Offset, Rel, j, '');
              end
            else
              begin
                ID := 0;
                Offset := 0;
                Rel := baseModule + 0;
                IDC := MakeComment('ID'+IntToStr(adr.ID), j, adr.ID);
              end;
          if (j mod 1000)=0 then Write('.');
          Inc(adr.ID);
          Inc(j);
        until j >= k else
        repeat
          fad.Read(adr.ID, sizeof(int64));
          fad.Read(adr.Offset, sizeof(int64));
          with far[i, j] do
          begin
            ID := adr.ID;
            Offset := adr.offset;
            Rel := baseModule + adr.offset;
            IDC := MakeName('ID'+IntToStr(ID), ID, Offset, Rel, j, '');
          end;
          if (j mod 1000)=0 then Write('.');
          Inc(j);
        until j >= k;
        SetLength(far[i], j);
        FreeAndNil(fad);
        WriteLN;
        WriteLN(#9#9+'Count='+IntToStr(j) + ' ' + IntToStr(j*16+sizeof(h)+4));
        WriteLN;
        Inc(i);
      except
        WriteLN(#9#9+'Error reading data');
      end;

      try
        Idents := TStringList.Create;
        DoParseIDs;

        var fcsv : Text;
        if  SourceVer = '*' then
        begin
          AssignFile(fcsv, SourceDir + '\AllVersions.csv');
          Rewrite(fcsv);
          WriteLN(fcsv, prologueCsv);
        end;
        if not DirectoryExists(SourceDir + '\idc') then TDirectory.CreateDirectory(SourceDir + '\idc');

        try
          IDCs := TStringList.Create;
          try
            var fi : Text;
            var fc : Text;
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
                  AssignFile(fc, SourceDir+'\idc\'+adVersions[i]+'\'+ChangeFileExt(adVersions[i], '.csv'));
                  Rewrite(fc);
                  WriteLN(fc, PrologueCsv);
                  var j : integer;
                  for j := 0 to Length(far[i])-1 do
                    begin
                      WriteLN(fi, #9+far[i, j].IDC);
                      if IsOpen(fcsv) then
                        WriteLN(fcsv, MakeCsv(adVersions, i, j, far));
                      WriteLN(fc, MakeCsv(adVersions, i, j, far));
                      if (j mod 1000)=0 then Write('.');
                      if (j>0) and ((j mod CountPerFile)=0) then
                      begin
                        WriteLN;
                        WriteLN(fi, epilogue);
                        Close(fi);
                        version := ExtractFileName(adVersions[i]);
                        version := ChangeFileExt(version, '')+'_'+IntToStr(j);
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
                          if (far[i,j].ID > 0) and (Int64(Idents.Objects[k])=far[i, j].ID) then
                          begin
                            IDCs.Add(MakeName(Idents[k], far[i, j].ID, far[i,j].Offset, far[i,j].Rel, j, #9+'ID:'+IntToStr(far[i,j].ID)));
                            Idents.Objects[k] := TObject(0);
                          end;
                        end;
                      end;
                    end;
                finally
                  WriteLN(fi, epilogue);
                  Close(fi);
                  WriteLN(fc, epilogueCsv);
                  Close(fc);
                  WriteLN;
                end;
              except
              end;
          finally
            if  SourceVer = '*' then begin
              WriteLN(fcsv, epilogueCsv);
              Close(fcsv);
            end;
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
                IDCs.Add(MakeComment(Idents[i], i, Int64(Idents.Objects[i])));
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
      Writeln(#13#13+E.ClassName, ': ', E.Message);
  end;
end.
