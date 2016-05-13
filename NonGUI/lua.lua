local Ui = NonGUI
setmetatable(Ui, Ui.CONSTANT)

local text = Ui.NewText{
    OnChar = function(text, ch, ctrl, alt, shift)
        print(ch, ctrl, alt, shift) return true
    end,
    OnEnter = function(text)
        print 'OnEnter'
    end,
    Style = Ui.wxTE_MULTILINE,
    Width = 100, Height = 100 } 

Ui.AddPane(text, Ui.wxRIGHT)
