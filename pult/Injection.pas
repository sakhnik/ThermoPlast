unit Injection;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls;

type
  TFormInjection = class(TForm)
    LInjection: TLabel;
    Linj1: TLabel;
    LInj2: TLabel;
    LInj3: TLabel;
    LQ: TLabel;
    Lt: TLabel;
    ImgdX: TImage;
    LLoad1: TLabel;
    LLoad2: TLabel;
    LLoad3: TLabel;
    LForm1: TLabel;
    LForm2: TLabel;
    LDecompression: TLabel;
    EInj1Q: TEdit;
    EInj2Q: TEdit;
    EInj3Q: TEdit;
    EFor1Q: TEdit;
    EFor2Q: TEdit;
    ELoad1Q: TEdit;
    ELoad2Q: TEdit;
    ELoad3Q: TEdit;
    EDecompQ: TEdit;
    EInjt: TEdit;
    EFor1t: TEdit;
    EFor2t: TEdit;
    EDecompt: TEdit;
    ELoadt: TEdit;
    EInj1x: TEdit;
    EInj3x: TEdit;
    EFor1x: TEdit;
    EFor2x: TEdit;
    ELoad1x: TEdit;
    ELoad2x: TEdit;
    ELoad3x: TEdit;
    EDecompx: TEdit;
    EInj2x: TEdit;
    ImgExit: TImage;
    BExit: TButton;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  FormInjection: TFormInjection;

implementation

{$R *.dfm}

end.
