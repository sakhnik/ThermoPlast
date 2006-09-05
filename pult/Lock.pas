unit Lock;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls;

type
  TFormLock = class(TForm)
    LTitle: TLabel;
    LQ: TLabel;
    LStart: TLabel;
    LAccel1: TLabel;
    LProtection: TLabel;
    LFullLock: TLabel;
    LStart1: TLabel;
    LAccel: TLabel;
    LDecel: TLabel;
    EStartQ: TEdit;
    EAccelQ: TEdit;
    EProtectQ: TEdit;
    EFullLockQ: TEdit;
    EAccelQ2: TEdit;
    EStartQ2: TEdit;
    ImgdX: TImage;
    EStartx: TEdit;
    EAccelx: TEdit;
    EProtectx: TEdit;
    EFullLockx: TEdit;
    EStartx2: TEdit;
    EAccelx2: TEdit;
    LDegas: TLabel;
    LContrLockTime: TLabel;
    LPushMov: TLabel;
    EDecelQ: TEdit;
    EDecelx: TEdit;
    LChopMov: TLabel;
    LRetChop: TLabel;
    EMovDropQ: TEdit;
    EMovDropx: TEdit;
    EBackDownQ: TEdit;
    EBackDownx: TEdit;
    EDegasx: TEdit;
    ImgClock: TImage;
    EDegast: TEdit;
    EPushMov: TEdit;
    ETimeTestt: TEdit;
    CBDif: TCheckBox;
    BExit: TButton;
    ImgLock: TImage;
    LChopMoves: TLabel;
    EChopMov: TEdit;
    BHelp: TButton;
    ImgHelp: TImage;
    ImgExit: TImage;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  FormLock: TFormLock;

implementation

{$R *.dfm}

end.
