unit Main;

interface

uses
  SysUtils, WinTypes, WinProcs, Messages, Classes, Graphics, Controls,
  Forms, Dialogs, StdCtrls, Buttons, FileCtrl, ExtCtrls,
  Status, Save, Menus,About;

type
  TMainForm = class(TForm)
    GroupBox1: TGroupBox;
    DirectoryListBox1: TDirectoryListBox;
    ListBox1: TListBox;
    DriveComboBox1: TDriveComboBox;
    BitBtn1: TBitBtn;
    BitBtn2: TBitBtn;
    BitBtn3: TBitBtn;
    GroupBox2: TGroupBox;
    ListBox2: TListBox;
    Label4: TLabel;
    GroupBox3: TGroupBox;
    Edit1: TEdit;
    BitBtn8: TBitBtn;
    BitBtn5: TBitBtn;
    MainMenu1: TMainMenu;
    File1: TMenuItem;
    exit1: TMenuItem;
    Help1: TMenuItem;
    Panel1: TPanel;
    Panel2: TPanel;
    Label2: TLabel;
    Panel3: TPanel;
    Label3: TLabel;
    About1: TMenuItem;
    Options1: TMenuItem;
    dh: TMenuItem;
    ch: TMenuItem;
    Label1: TLabel;
    procedure FormActivate(Sender: TObject);
    procedure DirectoryListBox1Change(Sender: TObject);
    procedure BitBtn6Click(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    procedure BitBtn2Click(Sender: TObject);
    procedure BitBtn3Click(Sender: TObject);
    procedure BitBtn7Click(Sender: TObject);
    procedure BitBtn8Click(Sender: TObject);
    procedure BitBtn5Click(Sender: TObject);
    procedure exit1Click(Sender: TObject);
    procedure About1Click(Sender: TObject);
    procedure ListBox1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  MainForm: TMainForm;
  stopex: Boolean;
  fout: TextFile;

implementation

{$R *.DFM}
var
   totbyte: Double;

function msb2d (const i:Longint): Double;
type
 foo = array[0..3] of byte;{}
{ foo = array[0..3] of shortint;  {}
var
   exponent: Longint;
   sign: Integer;
   p: ^foo;
   j,k: Longint;
   x : double;
begin
   k := 1;
   p := @i;
   exponent := p^[3] - $98;
   sign := 1;
   if (i and  $00800000) <> 0 then
      sign := -1;
   j := i or $00800000;
{   write(fout ,format('%12d ',[j])); {}
   j := j and $00FFFFFF;

{   writeln(fout ,format('%d, %d, %d, %d',
         [p^[0], p^[1], p^[2], p^[3]])); {}

{   write(fout ,format('%12d, %12d,   %d, %d',
         [i, j, sign, exponent])); {}

   if (exponent <= 32) and (exponent > -32) then
      begin
      if (exponent >= 0) then
         x := k shl exponent
      else
         x := k shl -exponent;

{   writeln(fout ,format('%.3f, %.3f ',
         [x, (sign*j/x)])); {}

      Result := sign * j / x;
      end
   else
      begin
      x := Exp(exponent*Ln(2.0));
{   writeln(fout ,format('%.3f, %.3f ',
         [x, (sign *j*x)])); {}
      Result := sign * j *x;
      end;
end;

procedure ExtractFile (const s: string; const p: string);
var
   fin: File;
{   fout: TextFile;    {}
   ss: String;
   cbuf: array[0..24] of char;
   jbuf: array[0..10] of Integer;
   lbuf: array[0..5] of Longint;
   i: integer;
   v: array[0..5] of double;
   xxx: array [0..3] of char;
begin
   ss := ExtractFileName(s);
   delete(ss,Pos('.NDX',ss),4);
   ss := p + '\'  + ss + '.csv';
   AssignFile(fin,s);
   reset(fin,24);
   AssignFile(fout,ss);
   rewrite(fout);
   BlockRead(fin,cbuf,1); {first header}
   cbuf[24] := Chr(0);
   if Mainform.dh.checked then
      Writeln(fout,'"'+ StrPas(cbuf) + '"');
   BlockRead(fin,jbuf,1);

{writeln(fout,format('C %d,  J %d,   L%d ',
         [sizeof(cbuf), sizeof(jbuf), sizeof(lbuf)]));
writeln(fout,format('%d, %d, %d %d %d %d',
         [jbuf[0], jbuf[1], jbuf[2], jbuf[3], jbuf[4], jbuf[5]]));
writeln(fout,format('%d, %d, %d %d %d ',
         [jbuf[6], jbuf[7], jbuf[8], jbuf[9], jbuf[10]]));    {}

{   BlockRead(fin,xxx,1);
writeln(fout,format('%d, %d, %d %d',
         [ord(xxx[0]), ord(xxx[1]), ord(xxx[2]), ord(xxx[3])])); {}

   if MainForm.ch.checked then
      writeln(fout,'"date","open","high","low","close","vol"');
   while not Eof(fin) do
      begin
      BlockRead(fin,lbuf,1);


      for i := 0 to 5 do
         v[i] := msb2d(lbuf[i]);

{      writeln(fout,format('%d, %d, %d, %d, %d, %d',
         [lbuf[0], lbuf[1], lbuf[2], lbuf[3], lbuf[4], lbuf[5]])); {}

      writeln(fout,format('%d,%.3f,%.3f,%.3f,%.3f,%d',
         [Trunc(v[0]),   {date as YYMMDD}
          v[5],          {open}
          v[1],          {high}
          v[2],          {low}
          v[3],          {close}
          Trunc(v[4])*100]));
      end;
   CloseFile(fout);
   CloseFile(fin);
end;

procedure TMainForm.FormActivate(Sender: TObject);
begin
   totbyte := 0;
   DriveComboBox1.ItemIndex := DriveComboBox1.Items.Count -1;
   DriveComboBox1.ItemIndex :=2;
   DirectoryListBox1.Drive :=
      DriveComboBox1.Items.Strings[DriveComboBox1.ItemIndex][1];
   Edit1.Text := SaveForm.Label1.Caption;
end;

procedure TMainForm.DirectoryListBox1Change(Sender: TObject);
var
   f: TSearchRec;
   e: Integer;
begin
   ListBox1.Clear;
   e := FindFirst(DirectoryListBox1.Directory + '\*.ndx',
         faAnyFile,f);
   while e = 0 do begin
      ListBox1.Items.Add(f.Name);
      e := FindNext(f);
      end;
end;

procedure TMainForm.BitBtn6Click(Sender: TObject);
begin
   Close;
end;

procedure TMainForm.BitBtn1Click(Sender: TObject);
var
   i: Integer;
begin
   if ListBox1.Items.Count <= 0 then exit;
   ListBox1.Items.BeginUpdate;
   for i := 0 to ListBox1.Items.Count -1 do
      ListBox1.Selected[i] := True;
   ListBox1.Items.EndUpdate;
   Label1.Caption := '  '+IntToStr(ListBox1.Items.Count-1)+
      ' Selected';
end;

procedure TMainForm.BitBtn2Click(Sender: TObject);
var
   i: Integer;
begin
   if ListBox1.Items.Count <= 0 then exit;
   for i := 0 to ListBox1.Items.Count -1 do
      ListBox1.Selected[i] := False;
   Label1.Caption := '  0 Selected'
end;

procedure TMainForm.BitBtn3Click(Sender: TObject);
var
   i: Integer;
   s: String;
   f: TSearchRec;
begin
   if ListBox1.Items.Count <= 0 then exit;
   Screen.Cursor := crHourglass;
   for i := 0 to ListBox1.Items.Count -1 do
      if ListBox1.Selected[i] then
        begin
        s := Label4.Caption + '\' +ListBox1.Items.Strings[i];
        if ListBox2.Items.IndexOf(s) = -1 then
           begin
           ListBox2.Items.Add(s);
           ListBox2.Update;
           if FindFirst(s,faAnyFile,f) = 0 then
              totbyte := totbyte + ((f.Size div 24)*44);
           end;
        Label3.Caption := '  '+IntToStr(ListBox2.Items.Count) +
          ' file(s) selected for extraction';
        Label2.Caption := '  '+Format( '%.0n',[totbyte]) +
          ' bytes (estimated) in output file(s)';
        Label3.Update;
        Label2.Update;
        ListBox1.Selected[i] := False;
        end;
      Screen.Cursor := crDefault;
      Label1.Caption := '  0 Selected'
end;

procedure TMainForm.BitBtn7Click(Sender: TObject);
var
   i: Integer;
   f: TSearchRec;
begin
  if ListBox1.Items.Count <= 0 then Exit;
   ProgForm.Gauge1.MaxValue := ListBox1.Items.Count;


end;

procedure TMainForm.BitBtn8Click(Sender: TObject);
begin
   if SaveForm.ShowModal = mrOk then
      Edit1.Text := SaveForm.Label1.Caption;
end;

procedure TMainForm.BitBtn5Click(Sender: TObject);
var
   i: Integer;
begin
    if ListBox2.Items.Count <= 0 then Exit;
    Progform.Gauge1.MaxValue := ListBox2.Items.Count;
    Cursor := crHourGlass;
    stopex := False;
    ProgForm.Show;
    for i := 0 to ListBox2.Items.Count -1 do
       begin
       Application.ProcessMessages;
       if stopex then break;
       ProgForm.Label1.Caption := 'Current File: '+ ListBox2.Items.Strings[i];
       ExtractFile(ListBox2.Items.Strings[i],Edit1.Text);
       ProgForm.Gauge1.Progress := i +1;
       ProgForm.Update;
       end;
    Cursor := crDefault;
    ProgForm.Hide;
    ListBox2.Items.Clear;
    Label1.Caption := '  0 Selected';
    Label3.Caption := '  0 file(s) selected for extraction';
    Label2.Caption := '  0 bytes (estimated) in output file(s)';
end;

procedure TMainForm.exit1Click(Sender: TObject);
begin
   Close;
end;

procedure TMainForm.About1Click(Sender: TObject);
begin
   AboutBox.Show;
end;

procedure TMainForm.ListBox1Click(Sender: TObject);
var
   i,n: Integer;
begin
   n := 0;
   for i := 0 to ListBox1.Items.Count -1 do
      if Listbox1.Selected[i] then Inc(n);
   
   Label1.Caption := '  '+IntTOStr(n)+' Selected';
end;

end.
