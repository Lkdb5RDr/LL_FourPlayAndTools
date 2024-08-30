program SynchroLink;

{$APPTYPE CONSOLE}

{$R *.res}

uses
  WinApi.Windows,
  System.SysUtils,
  System.IOUtils,
  CRCUnit in 'CRCUnit.pas';

function FindFirstFileNameW(lpFileName: PWideChar; dwFlags: Cardinal; var StringLength: Cardinal;
  var LinkName: array of WideChar; LinkNameLength: Cardinal): THandle;
  stdcall; external 'Kernel32.dll';

function FindNextFileNameW(hFindStream: THandle; var StringLength: Cardinal;
  var LinkName: array of WideChar; LinkNameLength: Cardinal): BOOL;
  stdcall; external 'Kernel32.dll';

Procedure FindCloseW(hFindStream: THandle); stdcall; external 'Kernel32.dll' name 'FindClose';

var
  lastParam: integer;
  mode: Integer;
    // 1: Redo: First, check destination for files in source. If they are not hardlinks of the same file, delete them.
    // 2: Space: First check destination for files in source. If they are not hardlinks of the same file but their content
    //      is identical delete them.
    // 3: New: Hardink files in source not in destination. Combine with a "diff" delete same to save space until 1 is actually
    //      implemented.
    // 4: Backup: Hardlink files in source not in destination from reference. (with or without purge). Don't copy if already linked.
  reference: String;
    //  If source and destination are not on the same disk, use reference as an earlier backup of source on the same drive
    //    as destination.
    // 7: Full: Full hardlinks of every file in source to destination (with or without purge)
	//
	// a mode is now required to avoid the accidents that hapened when the default was /FULL :)
function SizeOfFile(name: string): UInt64;
var
  F: file;
begin
  Result := 0;
  FileMode := 0;
  {$I-}
  AssignFile(F, name);
  Reset(F, 1);
  if IOResult = 0 then
    Result := FileSize(F);
  CloseFile(f);
  {$I+}
end;

procedure FlipSeparator(var s: string; toC: boolean = true);
var
  i: Integer;
begin
  if toC then begin
    for i := 1 to Length(s) do if s[i] = '\' then s[i] := '/';
  end else
    for i := 1 to Length(s) do if s[i] = '/' then s[i] := '\';
end;

procedure RemoveDrive(var s: string; toC: boolean = true);
begin
  if (Length(s) > 1) and (s[2] = ':') then Delete(s, 1, 2);
end;

function IsSameLink(SourceFilePath: string; DestinationFilePath: string): Boolean;
var
  found: Boolean;
  H: THandle;
  N: array [0..MAX_PATH] of WideChar;
  S: Cardinal;
  // P: PChar;
begin
  found := False;
  // H := 0;
  N := '';
  S := 4095;
  // P := @N;
  if FileExists(SourceFilePath) and FileExists(DestinationFilePath) then begin
    FlipSeparator(SourceFilePath);
    RemoveDrive(DestinationFilePath);
    H := FindFirstFileNameW(PWideChar(SourceFilePath), 0, S, N, Pred(Length(N)));
    if H <> INVALID_HANDLE_VALUE then try
      repeat
        found := SameText(N, DestinationFilePath);
        if found then
          Break;
        S := 4095;
      until not FindNextFileNameW(H, S, N, Pred(Length(N)));
    finally
      FindCloseW(H);
    end;
  end;
  Result := found;
end;

function IsSameContent(SourceFilePath: string; DestinationFilePath: string): Boolean;
var
  sc, dc: dword;
begin
  Result := False;
  if FileExists(SourceFilePath) and FileExists(DestinationFilePath) and
     (SizeOfFile(SourceFilePath) = SizeOfFile(DestinationFilePath)) then begin
    sc := CalcCRC32(SourceFilePath);
    dc := CalcCRC32(DestinationFilePath);
    Result := sc = dc;
  end;
end;

procedure RemoveAllDir(base: string; l: string = '');
var
  R: TSearchRec;
begin
  base := ExpandFileName(ExtractFilePath(base + '\.'));
  if FindFirst(base + l + '*.*', faAnyFile, R) = 0 then
    try
      repeat
        if (R.Attr and faDirectory) = faDirectory then
          if (R.Name <> '.') and (R.Name <> '..') then begin
            RemoveAllDir(base, l + R.Name + '\');
          end;
      until FindNext(R) <> 0;
    finally
      FindClose(R);
    end;
  if FindFirst(base + l + '*.*', faAnyFile, R) = 0 then
    try
      repeat
        if (R.Attr and faDirectory) <> faDirectory then
          DeleteFile(base + l + R.Name);
      until FindNext(R) <> 0;
    finally
      FindClose(R);
    end;
  if l <> '' then
    RemoveDir(base);
end;

procedure RemoveAllExtra(source, destination: string; reference : string = ''; l: string = '');
var
  R: TSearchRec;
begin
  source := ExpandFileName(ExtractFilePath(source + '\.'));
  destination := ExpandFileName(ExtractFilePath(destination + '\.'));
  if reference <> '' then reference := ExpandFileName(ExtractFilePath(reference + '\.'));
  if FindFirst(destination + l + '*.*', faAnyFile, R) = 0 then
    try
      repeat
        if (R.Attr and faDirectory) = faDirectory then
          if (R.Name <> '.') and (R.Name <> '..') then begin
            RemoveAllExtra(source, destination, reference, l + R.Name + '\');
          end;
      until FindNext(R) <> 0;
    finally
      FindClose(R);
    end;
  if FindFirst(destination + l + '*.*', faAnyFile, R) = 0 then
    try
      repeat
        if (R.Attr and faDirectory) <> faDirectory then begin
          if (mode in [4]) then begin  // Reference backup
            if FileExists(source + l + R.Name) and FileExists(reference + l + R.Name) then
              if not IsSameLink(PWideChar(reference + l + R.Name), PWideChar(destination + l + R.Name)) or
                 not IsSameContent(PWideChar(source + l + R.Name), PWideChar(reference + l + R.Name)) then
                if not DeleteFile(destination + l + R.Name) then
                  WriteLN('Error deleting different ' + destination + l + R.Name);
          end else if (mode in [5]) then begin  // backup
            if FileExists(source + l + R.Name) and FileExists(destination + l + R.Name) then
              if IsSameLink(PWideChar(source + l + R.Name), PWideChar(destination + l + R.Name)) and
                 not IsSameContent(PWideChar(source + l + R.Name), PWideChar(destination + l + R.Name)) then
                if not DeleteFile(destination + l + R.Name) then
                  WriteLN('Error deleting different ' + destination + l + R.Name);
          end else if (mode in [1]) then  // Redo Dir
            if not IsSameLink(PWideChar(source + l + R.Name), PWideChar(destination + l + R.Name)) then
              if not DeleteFile(destination + l + R.Name) then
                WriteLN('Error deleting different ' + destination + l + R.Name);
        end;
       until FindNext(R) <> 0;
    finally
      FindClose(R);
    end;
end;

var
  HardLinkCount : integer = 0;
  loops         : integer = 0;
  maxLoops      : integer = 1000;

procedure LinkDirIn(s: string; d: string; l: string = ''; x: string = '');
var
  R: TSearchRec;
  f: string;
begin
  s := ExpandFileName(ExtractFilePath(s + '\.'));
  d := ExpandFileName(ExtractFilePath(d + '\.'));
  if x <> '' then x := ExpandFileName(ExtractFilePath(x + '\.'));
  if FindFirst(s + l + '*.*', faAnyFile, R) = 0 then
    try
      repeat
        f := ExpandFileName(d + l + R.Name);
        if (R.Attr and faDirectory) = faDirectory then
          if (R.Name <> '.') and (R.Name <> '..') then try
            if not DirectoryExists(f) then
              CreateDir(f);
            LinkDirIn(s, d, l + R.Name + '\', x);
          except
          end;
        if (R.Attr and faDirectory) <> faDirectory then try
          if (mode in [2]) then begin  // saves space
            if FileExists(f) then
              if not IsSameLink(PWideChar(s + l + R.Name), PWideChar(f)) and
                     IsSameContent(PWideChar(s + l + R.Name), PWideChar(f)) then
                if not DeleteFile(f) then
                  WriteLN('Error deleting duplicate ' + f)
                else begin
                  loops := 0;
                  while FileExists(f) and (loops < maxLoops) do Sleep(100);

                  if CreateHardLink(PWideChar(f), PWideChar(s + l + R.Name), nil) then
                    Inc(HardLinkCount)
                  else begin
                    var ee : string := IntToStr(GetLastError);
                    WriteLN('Error creating hardlink ' + f + ' error : ' + ee);
                  end;

                end;
          end else if (mode in [6]) then begin  // checks link
            if FileExists(f) then
              if IsSameLink(PWideChar(s + l + R.Name), PWideChar(f)) then
                WriteLN('Is linked to ' + f);
          end else begin
            if FileExists(f) and (Mode in [7]) then
                DeleteFile(f);
            if not FileExists(f) then
              if x = '' then begin
                CreateHardLink(PWideChar(f), PWideChar(s + l + R.Name), nil);
                Inc(HardLinkCount);
              end else if FileExists(ExpandFileName(x + l + R.Name)) then begin
                if CreateHardLink(PWideChar(f), PWideChar(x + l + R.Name), nil) then
                  Inc(HardLinkCount)
                else begin
                  var ee : string := IntToStr(GetLastError);
                  WriteLN('Error creating hardlink ' + x + l + R.Name + ' error : ' + ee);
                end;
              end;
          end;
        except
        end;
      until FindNext(R) <> 0;
    finally
      FindClose(R);
    end;
end;

begin
  try
    { TODO -oUser -cConsole Main : Insert code here }
    Mode := 0;
    LastParam := 0;
    reference := '';
    // Required parameters: Source Destination
    if (ParamCount > 1) and not DirectoryExists(ExpandFileName(ParamStr(2))) then
       MkDir(ExpandFileName(ParamStr(2)));
    if (ParamCount < 2) or
       not DirectoryExists(ExpandFileName(ParamStr(1))) or
       not DirectoryExists(ExpandFileName(ParamStr(2))) then
      Writeln('Syntax error! Source and/or Destination not specified or does not exists')
    else begin
      if (ParamCount > (2+LastParam)) and (SameText(ParamStr(3+LastParam), '/PURGEDIR')) then begin
        RemoveAllDir(ParamStr(2));
        Inc(LastParam);
      end;
      if (ParamCount > (2+LastParam)) and (SameText(ParamStr(3+LastParam), '/REDODIR')) then begin
        Mode := 1;
        RemoveAllExtra(ParamStr(1), ParamStr(2));
        Inc(LastParam);
      end;
      if (ParamCount > (2+LastParam)) and (SameText(ParamStr(3+LastParam), '/FULL')) then begin
        Mode := 7;
        Inc(LastParam);
      end;
      if (ParamCount > (2+LastParam)) and (SameText(ParamStr(3+LastParam), '/SPACE')) then begin
        Mode := 2;
        Inc(LastParam);
      end;
      if (ParamCount > (2+LastParam)) and (SameText(ParamStr(3+LastParam), '/NEW')) then begin
        Inc(LastParam);
        Mode := 3;
      end;
      if (ParamCount > (2+LastParam)) and (SameText(ParamStr(3+LastParam), '/BACKUP')) then begin
        Inc(LastParam);
        Mode := 5;
        RemoveAllExtra(ParamStr(1), ParamStr(2));
      end;
      if (ParamCount > (2+LastParam)) and (SameText(ParamStr(3+LastParam), '/CHECK')) then begin
        Inc(LastParam);
        Mode := 6;
      end;
      if (ParamCount > (2+LastParam+1)) and (SameText(ParamStr(3+LastParam), '/REFDIR')) then begin
        Mode := 4;
        while (ParamCount > (2+LastParam+1)) do begin
          reference := ParamStr(4+LastParam);
          RemoveAllExtra(ParamStr(1), ParamStr(2), reference);
          Inc(LastParam);
        end;
      end;
      if Mode <> 0 then LinkDirIn(ParamStr(1), ParamStr(2), '', reference);
    end;
    Writeln(IntToStr(HardLinkCount), ' hard links created.');
  except
    on E: Exception do
      Writeln(E.ClassName, ': ', E.Message);
  end;
end.

