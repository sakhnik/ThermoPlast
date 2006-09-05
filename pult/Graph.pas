unit Graph;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls;

type
  TFormGraph = class(TForm)
    CBZone1: TCheckBox;
    CBZone2: TCheckBox;
    CBZone3: TCheckBox;
    CBZone4: TCheckBox;
    CBZone5: TCheckBox;
    CBZone6: TCheckBox;
    CBZone7: TCheckBox;
    CBZone8: TCheckBox;
    LPeriod: TLabel;
    EInj1Q: TEdit;
    LUnit: TLabel;
    LTdT: TLabel;
    CheckBox1: TCheckBox;
    LTemperature: TLabel;
    ImgExit: TImage;
    BExit: TButton;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  FormGraph: TFormGraph;

implementation

{$R *.dfm}

end.
