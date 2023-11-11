<GameFile>
  <PropertyGroup Name="PayInit_Win" Type="Layer" ID="0b6ac019-a80e-478e-aeda-9a35e2791bd9" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="PayInit_Win" Tag="137" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="Panel_2" ActionTag="-132283864" Tag="149" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="204" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="Image_1" ActionTag="-691223650" Tag="139" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="187.0000" RightMargin="187.0000" TopMargin="209.0000" BottomMargin="209.0000" Scale9Enable="True" LeftEage="139" RightEage="139" TopEage="119" BottomEage="119" Scale9OriginX="139" Scale9OriginY="119" Scale9Width="145" Scale9Height="123" ctype="ImageViewObjectData">
            <Size X="650.0000" Y="350.0000" />
            <Children>
              <AbstractNodeData Name="Button_4" ActionTag="-1003922175" CallBackType="Click" CallBackName="onClose" Tag="152" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="600.0000" RightMargin="-50.0000" TopMargin="-50.0000" BottomMargin="300.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="70" Scale9Height="78" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="100.0000" Y="100.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="650.0000" Y="350.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="1.0000" Y="1.0000" />
                <PreSize X="0.1538" Y="0.2857" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/pay/pay_popup_close_button.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="message" ActionTag="947929255" Tag="141" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TopMargin="40.0000" BottomMargin="110.0000" IsCustomSize="True" FontSize="32" LabelText="Bạn sẽ không nhận được mã kích hoạt. Thiết bị này sẽ được tự động kích hoạt. Bạn có thể khôi phục gói mua trên các thiết bị iOS khác.&#xA;Chúng tôi sẽ kiểm tra gói mua và tổng số lượng thiết bị được kích hoạt." HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="650.0000" Y="200.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="325.0000" Y="210.0000" />
                <Scale ScaleX="0.9000" ScaleY="1.0000" />
                <CColor A="255" R="40" G="127" B="157" />
                <PrePosition X="0.5000" Y="0.6000" />
                <PreSize X="1.0000" Y="0.5714" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Italic.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_1" ActionTag="-1714365652" CallBackType="Click" CallBackName="onBuy" Tag="148" IconVisible="False" RightMargin="-5.0000" TopMargin="264.0000" BottomMargin="-5.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="494" Scale9Height="69" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="655.0000" Y="91.0000" />
                <Children>
                  <AbstractNodeData Name="btn_title" ActionTag="-1750545318" Tag="147" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="232.5000" RightMargin="232.5000" TopMargin="23.0000" BottomMargin="23.0000" FontSize="35" LabelText="Mua ngay" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="190.0000" Y="45.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="327.5000" Y="45.5000" />
                    <Scale ScaleX="0.9000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.2901" Y="0.4945" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Black.ttf" Plist="" />
                    <OutlineColor A="255" R="0" G="128" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position Y="-5.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition Y="-0.0143" />
                <PreSize X="1.0077" Y="0.2600" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="mjstory/popup/networkdown_doc_bt1.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/popup/networkdown_doc_bt1.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.6348" Y="0.4557" />
            <FileData Type="Normal" Path="mjstory/popup/update_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>