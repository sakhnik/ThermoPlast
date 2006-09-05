unit Temperature;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls;

type
  TFormT = class(TForm)
    LTemperature: TLabel;
    LC1: TLabel;
    LC2: TLabel;
    LC3: TLabel;
    LC4: TLabel;
    LC5: TLabel;
    LC6: TLabel;
    LC7: TLabel;
    LC8: TLabel;
    LCA: TLabel;
    LT: TLabel;
    EC1T: TEdit;
    EC2T: TEdit;
    EC3T: TEdit;
    EC4T: TEdit;
    EC5T: TEdit;
    EC6T: TEdit;
    EC7T: TEdit;
    EC8T: TEdit;
    ECAT: TEdit;
    ImgMD: TImage;
    ImgPD: TImage;
    EC1mdt: TEdit;
    EC1pdt: TEdit;
    EC2mdt: TEdit;
    EC3mdt: TEdit;
    EC8pdt: TEdit;
    ECApdt: TEdit;
    EC4mdt: TEdit;
    EC2pdt: TEdit;
    EC5mdt: TEdit;
    EC3pdt: TEdit;
    EC6mdt: TEdit;
    EC4pdt: TEdit;
    EC7mdt: TEdit;
    EC5pdt: TEdit;
    EC8mdt: TEdit;
    EC6pdt: TEdit;
    ECAmdt: TEdit;
    EC7pdt: TEdit;
    ImgTs: TImage;
    EC1Ts: TEdit;
    EC2Ts: TEdit;
    EC3Ts: TEdit;
    EC4Ts: TEdit;
    EC5Ts: TEdit;
    EC6Ts: TEdit;
    EC7Ts: TEdit;
    EC8Ts: TEdit;
    ECATs: TEdit;
    LMinOilT: TLabel;
    EMinOilT: TEdit;
    LOilCoolT: TLabel;
    EOilCoolT: TEdit;
    BPID: TButton;
    ImgPID: TImage;
    BExit: TButton;
    ImgExit: TImage;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  FormT: TFormT;

implementation

{$R *.dfm}

end.
