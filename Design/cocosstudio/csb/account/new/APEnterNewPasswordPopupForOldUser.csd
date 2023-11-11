<GameFile>
  <PropertyGroup Name="APEnterNewPasswordPopupForOldUser" Type="Layer" ID="cddb24bc-a533-4633-99ef-9b25ee15c23b" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="APEnterNewPasswordPopupForOldUser" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="popup_bg" ActionTag="-1700710237" Tag="2386" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="236.5000" RightMargin="236.5000" TopMargin="109.0000" BottomMargin="109.0000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="64" Scale9Height="52" ctype="ImageViewObjectData">
            <Size X="551.0000" Y="550.0000" />
            <Children>
              <AbstractNodeData Name="exit_button" ActionTag="-542687799" CallBackType="Click" CallBackName="onCancel" Tag="2387" IconVisible="False" LeftMargin="494.9928" RightMargin="-3.9928" TopMargin="-5.1839" BottomMargin="495.1839" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="60.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="-1156760525" Tag="3310" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="12.5000" RightMargin="12.5000" TopMargin="12.5000" BottomMargin="12.5000" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="21" Scale9Height="21" ctype="ImageViewObjectData">
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
                <Position X="524.9928" Y="525.1839" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9528" Y="0.9549" />
                <PreSize X="0.1089" Y="0.1091" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="lock_icon" ActionTag="1640414426" Tag="2389" IconVisible="False" PositionPercentXEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="229.4915" RightMargin="229.4915" TopMargin="22.0367" BottomMargin="435.9483" LeftEage="30" RightEage="30" TopEage="30" BottomEage="30" Scale9OriginX="30" Scale9OriginY="30" Scale9Width="32" Scale9Height="32" ctype="ImageViewObjectData">
                <Size X="92.0170" Y="92.0150" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="481.9558" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.8763" />
                <PreSize X="0.1670" Y="0.1673" />
                <FileData Type="Normal" Path="account/lock_icon.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="title" ActionTag="316645607" Tag="2390" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="46.0000" RightMargin="46.0000" TopMargin="122.0004" BottomMargin="325.9996" FontSize="28" LabelText="Your account does not have a&#xA;password.Please enter your new &#xA;password." HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="459.0000" Y="102.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="376.9996" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.6855" />
                <PreSize X="0.8330" Y="0.1855" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="password_textfield_bg" ActionTag="-891618756" Tag="2391" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="55.0000" RightMargin="55.0000" TopMargin="243.5000" BottomMargin="241.5000" Scale9Enable="True" LeftEage="11" RightEage="11" TopEage="7" BottomEage="7" Scale9OriginX="11" Scale9OriginY="7" Scale9Width="157" Scale9Height="70" ctype="ImageViewObjectData">
                <Size X="441.0000" Y="65.0000" />
                <Children>
                  <AbstractNodeData Name="password_textfield" ActionTag="1915613856" CallBackType="Event" Tag="2392" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="8.8200" RightMargin="8.8200" TopMargin="12.4995" BottomMargin="0.5005" TouchEnable="True" FontSize="29" IsCustomSize="True" LabelText="" PlaceHolderText="Nhập mật khẩu ..." MaxLengthText="10" PasswordEnable="True" ctype="TextFieldObjectData">
                    <Size X="423.3600" Y="52.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="220.5000" Y="26.5005" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="160" G="159" B="159" />
                    <PrePosition X="0.5000" Y="0.4077" />
                    <PreSize X="0.9600" Y="0.8000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="274.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4982" />
                <PreSize X="0.8004" Y="0.1182" />
                <FileData Type="Normal" Path="account/grey_textbox_bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="password_textfield_bg_0" ActionTag="-1978838513" Tag="2393" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="55.0000" RightMargin="55.0000" TopMargin="337.5000" BottomMargin="147.5000" Scale9Enable="True" LeftEage="11" RightEage="11" TopEage="7" BottomEage="7" Scale9OriginX="11" Scale9OriginY="7" Scale9Width="157" Scale9Height="70" ctype="ImageViewObjectData">
                <Size X="441.0000" Y="65.0000" />
                <Children>
                  <AbstractNodeData Name="reentering_password_textfield" ActionTag="570727211" CallBackType="Event" Tag="2394" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="8.8200" RightMargin="8.8200" TopMargin="12.4995" BottomMargin="0.5005" TouchEnable="True" FontSize="29" IsCustomSize="True" LabelText="" PlaceHolderText="Nhập lại mật khẩu ..." MaxLengthText="10" PasswordEnable="True" ctype="TextFieldObjectData">
                    <Size X="423.3600" Y="52.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="220.5000" Y="26.5005" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="160" G="159" B="159" />
                    <PrePosition X="0.5000" Y="0.4077" />
                    <PreSize X="0.9600" Y="0.8000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="180.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.3273" />
                <PreSize X="0.8004" Y="0.1182" />
                <FileData Type="Normal" Path="account/grey_textbox_bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="finish_button" ActionTag="-784347506" CallBackType="Click" CallBackName="onFinish" Tag="2395" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="177.5000" RightMargin="177.5000" TopMargin="445.5000" BottomMargin="55.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="22" RightEage="22" TopEage="22" BottomEage="22" Scale9OriginX="22" Scale9OriginY="22" Scale9Width="113" Scale9Height="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="196.0000" Y="49.0000" />
                <Children>
                  <AbstractNodeData Name="finish_button_title" ActionTag="-2052862798" Tag="2396" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="19.5000" RightMargin="19.5000" TopMargin="9.5000" BottomMargin="9.5000" FontSize="25" LabelText="Hoàn Thành" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="157.0000" Y="30.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="98.0000" Y="24.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.8010" Y="0.6122" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="80.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1455" />
                <PreSize X="0.3557" Y="0.0891" />
                <TextColor A="255" R="255" G="255" B="255" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="warning_label" ActionTag="83651721" Tag="2397" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="128.5200" RightMargin="106.4800" TopMargin="514.0000" BottomMargin="14.0000" FontSize="18" LabelText="Mật khẩu nhập phải ít nhất 8 ký tự" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="316.0000" Y="22.0000" />
                <Children>
                  <AbstractNodeData Name="icon_warning" ActionTag="-1253125033" Tag="2398" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="-24.0000" RightMargin="322.0000" TopMargin="3.0000" BottomMargin="3.0000" LeftEage="5" RightEage="5" TopEage="5" BottomEage="5" Scale9OriginX="5" Scale9OriginY="5" Scale9Width="8" Scale9Height="6" ctype="ImageViewObjectData">
                    <Size X="18.0000" Y="16.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="-15.0000" Y="11.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="-0.0475" Y="0.5000" />
                    <PreSize X="0.0570" Y="0.7273" />
                    <FileData Type="Normal" Path="license/active_license/activelicense_iconwarning.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="286.5200" Y="25.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="254" G="2" B="1" />
                <PrePosition X="0.5200" Y="0.0455" />
                <PreSize X="0.5735" Y="0.0400" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.5381" Y="0.7161" />
            <FileData Type="Normal" Path="account/white_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>