object FormGraph: TFormGraph
  Left = 192
  Top = 107
  Width = 280
  Height = 205
  Caption = 'FormGraph'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object LPeriod: TLabel
    Left = 88
    Top = 111
    Width = 136
    Height = 9
    AutoSize = False
    Caption = #1055#1077#1088#1080#1086#1076' '#1085#1072#1082#1086#1087#1083#1077#1085#1080#1103
    Enabled = False
  end
  object LUnit: TLabel
    Left = 264
    Top = 113
    Width = 8
    Height = 8
    AutoSize = False
    Caption = #1089
    Enabled = False
  end
  object LTdT: TLabel
    Left = 88
    Top = 123
    Width = 112
    Height = 8
    AutoSize = False
    Caption = #1054#1090#1086#1073#1088' '#1079#1085#1072#1095#1077#1085#1080#1103
    Enabled = False
  end
  object LTemperature: TLabel
    Left = 80
    Top = 1
    Width = 88
    Height = 16
    Alignment = taCenter
    AutoSize = False
    Caption = #1058#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072
    Enabled = False
  end
  object ImgExit: TImage
    Left = 256
    Top = 172
    Width = 16
    Height = 12
    Picture.Data = {
      07544269746D617072000000424D72000000000000003E000000280000001000
      00000D000000010001000000000034000000C40E0000C40E0000020000000000
      000000000000FFFFFF0003F00000E0F70000E0F70000E0F70000E0F70000E0F7
      0000E0F70000E0F70000E0070000FFFF0000F81F0000F81F0000F81F0000}
  end
  object CBZone1: TCheckBox
    Left = 16
    Top = 112
    Width = 60
    Height = 9
    Caption = #1047#1086#1085#1072' 1'
    TabOrder = 0
  end
  object CBZone2: TCheckBox
    Left = 16
    Top = 121
    Width = 60
    Height = 9
    Caption = #1047#1086#1085#1072' 2'
    TabOrder = 1
  end
  object CBZone3: TCheckBox
    Left = 16
    Top = 130
    Width = 60
    Height = 9
    Caption = #1047#1086#1085#1072' 3'
    TabOrder = 2
  end
  object CBZone4: TCheckBox
    Left = 16
    Top = 139
    Width = 60
    Height = 9
    Caption = #1047#1086#1085#1072' 4'
    TabOrder = 3
  end
  object CBZone5: TCheckBox
    Left = 16
    Top = 148
    Width = 60
    Height = 9
    Caption = #1047#1086#1085#1072' 5'
    TabOrder = 4
  end
  object CBZone6: TCheckBox
    Left = 16
    Top = 157
    Width = 60
    Height = 9
    Caption = #1047#1086#1085#1072' 6'
    TabOrder = 5
  end
  object CBZone7: TCheckBox
    Left = 16
    Top = 166
    Width = 60
    Height = 9
    Caption = #1047#1086#1085#1072' 7'
    TabOrder = 6
  end
  object CBZone8: TCheckBox
    Left = 16
    Top = 175
    Width = 60
    Height = 9
    Caption = #1047#1086#1085#1072' 8'
    TabOrder = 7
  end
  object EInj1Q: TEdit
    Left = 232
    Top = 110
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 8
    Text = '23'
  end
  object CheckBox1: TCheckBox
    Left = 216
    Top = 124
    Width = 44
    Height = 9
    Caption = 'T/dT'
    TabOrder = 9
  end
  object BExit: TButton
    Left = 224
    Top = 165
    Width = 48
    Height = 24
    Caption = #1042#1067#1061
    TabOrder = 10
  end
end
