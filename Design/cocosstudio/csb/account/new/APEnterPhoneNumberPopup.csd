<GameFile>
  <PropertyGroup Name="APEnterPhoneNumberPopup" Type="Layer" ID="b03b7d9e-f934-4a7f-b227-4330c87ce0fa" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="APEnterPhoneNumberPopup" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="popup_bg" ActionTag="-394346387" Tag="360" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="236.5000" RightMargin="236.5000" TopMargin="206.5000" BottomMargin="206.5000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="64" Scale9Height="52" ctype="ImageViewObjectData">
            <Size X="551.0000" Y="355.0000" />
            <Children>
              <AbstractNodeData Name="exit_button" ActionTag="-825665338" CallBackType="Click" CallBackName="onCancel" Tag="2047" IconVisible="False" LeftMargin="498.3162" RightMargin="-7.3162" TopMargin="-5.6340" BottomMargin="300.6340" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="60.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="-86301685" Tag="3265" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="12.5000" RightMargin="12.5000" TopMargin="12.5000" BottomMargin="12.5000" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="21" Scale9Height="21" ctype="ImageViewObjectData">
                    <Size X="35.0000" Y="35.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="30.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.5833" Y="0.5833" />
                    <FileData Type="Normal" Path="account/exit.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="528.3162" Y="330.6340" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9588" Y="0.9314" />
                <PreSize X="0.1089" Y="0.1690" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="title" ActionTag="1717790470" Tag="362" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="72.0000" RightMargin="72.0000" TopMargin="71.0000" BottomMargin="226.0000" FontSize="24" LabelText="Ba mẹ vui lòng nhập số điện thoại &#xA;để đăng ký sử dụng chương trình" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="407.0000" Y="58.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="255.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.7183" />
                <PreSize X="0.7387" Y="0.1634" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="textfield_bg" ActionTag="629959836" Tag="590" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="55.0000" RightMargin="55.0000" TopMargin="163.0000" BottomMargin="127.0000" Scale9Enable="True" LeftEage="11" RightEage="11" TopEage="7" BottomEage="7" Scale9OriginX="11" Scale9OriginY="7" Scale9Width="157" Scale9Height="70" ctype="ImageViewObjectData">
                <Size X="441.0000" Y="65.0000" />
                <Children>
                  <AbstractNodeData Name="phone_textfield" ActionTag="387899855" CallBackType="Event" Tag="591" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="8.8200" RightMargin="8.8200" TopMargin="12.4995" BottomMargin="0.5005" TouchEnable="True" FontSize="32" IsCustomSize="True" LabelText="" PlaceHolderText="Nhập số điện thoại..." MaxLengthText="10" ctype="TextFieldObjectData">
                    <Size X="423.3600" Y="52.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="220.5000" Y="26.5005" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="155" G="155" B="155" />
                    <PrePosition X="0.5000" Y="0.4077" />
                    <PreSize X="0.9600" Y="0.8000" />
                    <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="159.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4493" />
                <PreSize X="0.8004" Y="0.1831" />
                <FileData Type="Normal" Path="account/grey_textbox_bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="send_button" ActionTag="-1114202442" CallBackType="Click" CallBackName="onSend" Tag="647" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="197.0000" RightMargin="197.0000" TopMargin="253.5000" BottomMargin="55.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="22" RightEage="22" TopEage="22" BottomEage="22" Scale9OriginX="22" Scale9OriginY="22" Scale9Width="113" Scale9Height="2" DisplayState="False" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="157.0000" Y="46.0000" />
                <Children>
                  <AbstractNodeData Name="send_title" ActionTag="714587029" Tag="648" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="56.0000" RightMargin="56.0000" TopMargin="8.0000" BottomMargin="8.0000" FontSize="25" LabelText="Gửi" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="45.0000" Y="30.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="78.5000" Y="23.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.2866" Y="0.6522" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="78.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2211" />
                <PreSize X="0.2849" Y="0.1296" />
                <TextColor A="255" R="255" G="255" B="255" />
                <DisabledFileData Type="Normal" Path="account/button_grey.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="warning_label" ActionTag="-1421545581" VisibleForFrame="False" Tag="373" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="90.0200" RightMargin="67.9800" TopMargin="319.0000" BottomMargin="14.0000" FontSize="18" LabelText="Số Điện Thoại nhập sai hoặc không tồn tại !" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="393.0000" Y="22.0000" />
                <Children>
                  <AbstractNodeData Name="icon_warning" ActionTag="-1449716811" Tag="372" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="-24.0000" RightMargin="399.0000" TopMargin="3.0000" BottomMargin="3.0000" LeftEage="5" RightEage="5" TopEage="5" BottomEage="5" Scale9OriginX="5" Scale9OriginY="5" Scale9Width="8" Scale9Height="6" ctype="ImageViewObjectData">
                    <Size X="18.0000" Y="16.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="-15.0000" Y="11.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="-0.0382" Y="0.5000" />
                    <PreSize X="0.0458" Y="0.7273" />
                    <FileData Type="Normal" Path="license/active_license/activelicense_iconwarning.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="286.5200" Y="25.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="254" G="2" B="1" />
                <PrePosition X="0.5200" Y="0.0704" />
                <PreSize X="0.7132" Y="0.0620" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.2000" ScaleY="1.2000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.5381" Y="0.4622" />
            <FileData Type="Normal" Path="account/white_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>