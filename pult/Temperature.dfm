object FormT: TFormT
  Left = 206
  Top = 154
  Width = 200
  Height = 200
  Caption = 'Form'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object LTemperature: TLabel
    Left = 16
    Top = 1
    Width = 168
    Height = 16
    Alignment = taCenter
    AutoSize = False
    Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1090#1077#1084#1087#1077#1088#1072#1090#1091#1088#1099
    Enabled = False
  end
  object LC1: TLabel
    Left = 8
    Top = 33
    Width = 56
    Height = 8
    AutoSize = False
    Caption = #1050#1072#1085#1072#1083' 1'
    Enabled = False
  end
  object LC2: TLabel
    Left = 8
    Top = 43
    Width = 56
    Height = 8
    AutoSize = False
    Caption = #1050#1072#1085#1072#1083' 2'
    Enabled = False
  end
  object LC3: TLabel
    Left = 8
    Top = 53
    Width = 56
    Height = 8
    AutoSize = False
    Caption = #1050#1072#1085#1072#1083' 3'
    Enabled = False
  end
  object LC4: TLabel
    Left = 8
    Top = 63
    Width = 56
    Height = 8
    AutoSize = False
    Caption = #1050#1072#1085#1072#1083' 4'
    Enabled = False
  end
  object LC5: TLabel
    Left = 8
    Top = 73
    Width = 56
    Height = 8
    AutoSize = False
    Caption = #1050#1072#1085#1072#1083' 5'
    Enabled = False
  end
  object LC6: TLabel
    Left = 8
    Top = 83
    Width = 56
    Height = 8
    AutoSize = False
    Caption = #1050#1072#1085#1072#1083' 6'
    Enabled = False
  end
  object LC7: TLabel
    Left = 8
    Top = 93
    Width = 56
    Height = 8
    AutoSize = False
    Caption = #1050#1072#1085#1072#1083' 7'
    Enabled = False
  end
  object LC8: TLabel
    Left = 8
    Top = 103
    Width = 56
    Height = 8
    AutoSize = False
    Caption = #1050#1072#1085#1072#1083' 8'
    Enabled = False
  end
  object LCA: TLabel
    Left = 8
    Top = 114
    Width = 48
    Height = 8
    AutoSize = False
    Caption = #1042#1084#1077#1089#1090#1077
    Enabled = False
  end
  object LT: TLabel
    Left = 84
    Top = 16
    Width = 8
    Height = 16
    AutoSize = False
    Caption = 'T'
    Enabled = False
  end
  object ImgMD: TImage
    Left = 108
    Top = 19
    Width = 16
    Height = 10
    Picture.Data = {
      07544269746D617066000000424D66000000000000003E000000280000001000
      00000A000000010001000000000028000000C40E0000C40E0000020000000000
      000000000000FFFFFF00FC010000FDF90000FEF30000FEF30000036700000367
      0000FF8F0000FF8F0000FFDF0000FFDF0000}
  end
  object ImgPD: TImage
    Left = 132
    Top = 19
    Width = 16
    Height = 10
    Picture.Data = {
      07544269746D617066000000424D66000000000000003E000000280000001000
      00000A000000010001000000000028000000C40E0000C40E0000020000000000
      000000000000FFFFFF00FC010000FDF90000CEF30000CEF30000036700000367
      0000CF8F0000CF8F0000FFDF0000FFDF0000}
  end
  object ImgTs: TImage
    Left = 160
    Top = 16
    Width = 16
    Height = 14
    Picture.Data = {
      07544269746D617076000000424D76000000000000003E000000280000001000
      00000E000000010001000000000038000000C40E0000C40E0000020000000000
      000000000000FFFFFF00E1FF0000F3FF0000F3FF0000F3FF0000F3FF0000F3FF
      0000F3FF0000F3FF0000F3FF000033350000003B0000FFE00000FFFB0000FFF5
      0000}
  end
  object LMinOilT: TLabel
    Left = 8
    Top = 133
    Width = 136
    Height = 8
    AutoSize = False
    Caption = #1052#1080#1085#1080#1084#1072#1083#1100#1085' '#1058' '#1084#1072#1089#1083#1072
    Enabled = False
  end
  object LOilCoolT: TLabel
    Left = 8
    Top = 143
    Width = 112
    Height = 8
    AutoSize = False
    Caption = #1058' '#1086#1093#1083#1072#1078#1076' '#1084#1072#1089#1083#1072
    Enabled = False
  end
  object ImgPID: TImage
    Left = 32
    Top = 161
    Width = 20
    Height = 23
    Picture.Data = {
      07544269746D61709A000000424D9A000000000000003E000000280000001400
      00001700000001000100000000005C000000C40E0000C40E0000020000000000
      000000000000FFFFFF00FF0FF000C70E7000C30C30000000300000E0100008E1
      00001CF380000FFF00000F0F0000FCF3C0001AF50000CCF23000CEF530009EF6
      B00098F1300086F270009AF3F0009503F0009EB3F0009FB3F0008033F0008033
      F000FFF3F000}
    Visible = False
  end
  object ImgExit: TImage
    Left = 168
    Top = 166
    Width = 16
    Height = 12
    Picture.Data = {
      07544269746D617072000000424D72000000000000003E000000280000001000
      00000D000000010001000000000034000000C40E0000C40E0000020000000000
      000000000000FFFFFF0003F00000E0F70000E0F70000E0F70000E0F70000E0F7
      0000E0F70000E0F70000E0070000FFFF0000F81F0000F81F0000F81F0000}
  end
  object EC1T: TEdit
    Left = 72
    Top = 32
    Width = 32
    Height = 10
    AutoSize = False
    TabOrder = 0
    Text = '235'
  end
  object EC2T: TEdit
    Left = 72
    Top = 42
    Width = 32
    Height = 10
    AutoSize = False
    TabOrder = 1
    Text = '224'
  end
  object EC3T: TEdit
    Left = 72
    Top = 52
    Width = 32
    Height = 10
    AutoSize = False
    TabOrder = 2
    Text = '213'
  end
  object EC4T: TEdit
    Left = 72
    Top = 62
    Width = 32
    Height = 10
    AutoSize = False
    TabOrder = 3
    Text = '198'
  end
  object EC5T: TEdit
    Left = 72
    Top = 72
    Width = 32
    Height = 10
    AutoSize = False
    TabOrder = 4
    Text = '000'
  end
  object EC6T: TEdit
    Left = 72
    Top = 82
    Width = 32
    Height = 10
    AutoSize = False
    TabOrder = 5
    Text = '000'
  end
  object EC7T: TEdit
    Left = 72
    Top = 92
    Width = 32
    Height = 10
    AutoSize = False
    TabOrder = 6
    Text = '000'
  end
  object EC8T: TEdit
    Left = 72
    Top = 102
    Width = 32
    Height = 10
    AutoSize = False
    TabOrder = 7
    Text = '000'
  end
  object ECAT: TEdit
    Left = 72
    Top = 113
    Width = 32
    Height = 10
    AutoSize = False
    TabOrder = 8
    Text = '000'
  end
  object EC1mdt: TEdit
    Left = 104
    Top = 32
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 9
    Text = '10'
  end
  object EC1pdt: TEdit
    Left = 128
    Top = 32
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 10
    Text = '05'
  end
  object EC2mdt: TEdit
    Left = 104
    Top = 42
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 11
    Text = '11'
  end
  object EC3mdt: TEdit
    Left = 104
    Top = 52
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 12
    Text = '13'
  end
  object EC8pdt: TEdit
    Left = 128
    Top = 102
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 13
    Text = '00'
  end
  object ECApdt: TEdit
    Left = 128
    Top = 113
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 14
    Text = '00'
  end
  object EC4mdt: TEdit
    Left = 104
    Top = 62
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 15
    Text = '24'
  end
  object EC2pdt: TEdit
    Left = 128
    Top = 42
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 16
    Text = '09'
  end
  object EC5mdt: TEdit
    Left = 104
    Top = 72
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 17
    Text = '00'
  end
  object EC3pdt: TEdit
    Left = 128
    Top = 52
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 18
    Text = '12'
  end
  object EC6mdt: TEdit
    Left = 104
    Top = 82
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 19
    Text = '00'
  end
  object EC4pdt: TEdit
    Left = 128
    Top = 62
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 20
    Text = '25'
  end
  object EC7mdt: TEdit
    Left = 104
    Top = 92
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 21
    Text = '00'
  end
  object EC5pdt: TEdit
    Left = 128
    Top = 72
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 22
    Text = '00'
  end
  object EC8mdt: TEdit
    Left = 104
    Top = 102
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 23
    Text = '00'
  end
  object EC6pdt: TEdit
    Left = 128
    Top = 82
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 24
    Text = '00'
  end
  object ECAmdt: TEdit
    Left = 104
    Top = 113
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 25
    Text = '00'
  end
  object EC7pdt: TEdit
    Left = 128
    Top = 92
    Width = 24
    Height = 10
    AutoSize = False
    TabOrder = 26
    Text = '00'
  end
  object EC1Ts: TEdit
    Left = 152
    Top = 32
    Width = 32
    Height = 10
    AutoSize = False
    Enabled = False
    TabOrder = 27
    Text = '235'
  end
  object EC2Ts: TEdit
    Left = 152
    Top = 42
    Width = 32
    Height = 10
    AutoSize = False
    Enabled = False
    TabOrder = 28
    Text = '224'
  end
  object EC3Ts: TEdit
    Left = 152
    Top = 52
    Width = 32
    Height = 10
    AutoSize = False
    Enabled = False
    TabOrder = 29
    Text = '213'
  end
  object EC4Ts: TEdit
    Left = 152
    Top = 62
    Width = 32
    Height = 10
    AutoSize = False
    Enabled = False
    TabOrder = 30
    Text = '198'
  end
  object EC5Ts: TEdit
    Left = 152
    Top = 72
    Width = 32
    Height = 10
    AutoSize = False
    Enabled = False
    TabOrder = 31
    Text = '000'
  end
  object EC6Ts: TEdit
    Left = 152
    Top = 82
    Width = 32
    Height = 10
    AutoSize = False
    Enabled = False
    TabOrder = 32
    Text = '000'
  end
  object EC7Ts: TEdit
    Left = 152
    Top = 92
    Width = 32
    Height = 10
    AutoSize = False
    Enabled = False
    TabOrder = 33
    Text = '000'
  end
  object EC8Ts: TEdit
    Left = 152
    Top = 102
    Width = 32
    Height = 10
    AutoSize = False
    Enabled = False
    TabOrder = 34
    Text = '000'
  end
  object ECATs: TEdit
    Left = 152
    Top = 113
    Width = 32
    Height = 10
    AutoSize = False
    Enabled = False
    TabOrder = 35
    Text = '000'
  end
  object EMinOilT: TEdit
    Left = 152
    Top = 132
    Width = 32
    Height = 10
    AutoSize = False
    TabOrder = 36
    Text = '235'
  end
  object EOilCoolT: TEdit
    Left = 152
    Top = 142
    Width = 32
    Height = 10
    AutoSize = False
    TabOrder = 37
    Text = '235'
  end
  object BPID: TButton
    Left = 8
    Top = 160
    Width = 48
    Height = 24
    Caption = #1055#1048#1044
    TabOrder = 38
    Visible = False
  end
  object BExit: TButton
    Left = 136
    Top = 160
    Width = 48
    Height = 25
    Caption = #1042#1067#1061
    TabOrder = 39
  end
end
