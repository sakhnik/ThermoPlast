unit machine;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, StdCtrls;

type
  TFormMachine = class(TForm)
    ImgPlate1_1: TImage;
    ImgPlate1_2: TImage;
    ImgPlate1_3: TImage;
    ImgPlate2: TImage;
    ImgCanon: TImage;
    ImgScrew: TImage;
    LT4: TLabel;
    LT3: TLabel;
    LT2: TLabel;
    Ldx1: TLabel;
    Ldx3: TLabel;
    LdT4: TLabel;
    LdT3: TLabel;
    LdT2: TLabel;
    LdT1: TLabel;
    LTS4: TLabel;
    LTS3: TLabel;
    LTS2: TLabel;
    LTS1: TLabel;
    Img31: TImage;
    Img32: TImage;
    Img33: TImage;
    Img36: TImage;
    Img35: TImage;
    Img34: TImage;
    Img8: TImage;
    Img9: TImage;
    Img28: TImage;
    Img13: TImage;
    LTotal: TLabel;
    LTCount: TLabel;
    ImgEliminator: TImage;
    ImgOil: TImage;
    LOil: TLabel;
    LTS5: TLabel;
    LdT5: TLabel;
    LT5: TLabel;
    LTS6: TLabel;
    LdT6: TLabel;
    LT6: TLabel;
    LTS7: TLabel;
    LdT7: TLabel;
    LT7: TLabel;
    LT1: TLabel;
    LN7: TLabel;
    LN6: TLabel;
    LN5: TLabel;
    LN4: TLabel;
    LN3: TLabel;
    LN2: TLabel;
    LN1: TLabel;
    Img12: TImage;
    Img6: TImage;
    Img5: TImage;
    Img3: TImage;
    Img4: TImage;
    Img37: TImage;
    Img38: TImage;
    ImgClock: TImage;
    LClock: TLabel;
    BInjection: TButton;
    BLock: TButton;
    ImgInjection: TImage;
    ImgLock: TImage;
    BThermo: TButton;
    ImgThermo: TImage;
    BTest: TButton;
    ImgTest: TImage;
    Img2: TImage;
    Img1: TImage;
    Img10: TImage;
    Img14: TImage;
    BHelp: TButton;
    ImgHelp: TImage;
    LMessage: TLabel;
    ImgPanel: TImage;
    ImgProp: TImage;
    BProp: TButton;
    Img1_1: TImage;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  FormMachine: TFormMachine;

implementation

{$R *.dfm}

end.
