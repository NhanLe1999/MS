<GameFile>
  <PropertyGroup Name="APRegisterPopup" Type="Layer" ID="70e66b74-8535-4c5b-92d7-d8bd1aff21f1" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="APRegisterPopup" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="popup_bg" ActionTag="-207189224" Tag="1789" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="236.5000" RightMargin="236.5000" TopMargin="114.0000" BottomMargin="114.0000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="10" BottomEage="10" Scale9OriginX="15" Scale9OriginY="10" Scale9Width="64" Scale9Height="62" ctype="ImageViewObjectData">
            <Size X="551.0000" Y="540.0000" />
            <Children>
              <AbstractNodeData Name="exit_button" ActionTag="-1396093896" CallBackType="Click" CallBackName="onCancel" Tag="2137" IconVisible="False" LeftMargin="498.0927" RightMargin="-7.0927" TopMargin="-5.5087" BottomMargin="485.5087" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="60.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="1451393185" Tag="3397" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="12.5000" RightMargin="12.5000" TopMargin="12.5000" BottomMargin="12.5000" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="21" Scale9Height="21" ctype="ImageViewObjectData">
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
                <Position X="528.0927" Y="515.5087" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9584" Y="0.9546" />
                <PreSize X="0.1089" Y="0.1111" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="title" ActionTag="1499723513" Tag="1791" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="34.5000" RightMargin="34.5000" TopMargin="47.9992" BottomMargin="424.0008" FontSize="28" LabelText="Ba mẹ vui lòng nhập mật khẩu để&#xA;đăng nhập vào ứng dụng" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="482.0000" Y="68.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="458.0008" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.8481" />
                <PreSize X="0.8748" Y="0.1259" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="input_textfield_bg" ActionTag="-2132174653" Tag="1792" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="55.0000" RightMargin="55.0000" TopMargin="149.4997" BottomMargin="325.5003" Scale9Enable="True" LeftEage="11" RightEage="11" TopEage="7" BottomEage="7" Scale9OriginX="11" Scale9OriginY="7" Scale9Width="157" Scale9Height="70" ctype="ImageViewObjectData">
                <Size X="441.0000" Y="65.0000" />
                <Children>
                  <AbstractNodeData Name="input_textfield" ActionTag="-833830603" CallBackType="Event" Tag="1793" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="8.8200" RightMargin="8.8200" TopMargin="12.4995" BottomMargin="0.5005" TouchEnable="True" FontSize="32" IsCustomSize="True" LabelText="" PlaceHolderText="" MaxLengthText="10" ctype="TextFieldObjectData">
                    <Size X="423.3600" Y="52.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="220.5000" Y="26.5005" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="0.5000" Y="0.4077" />
                    <PreSize X="0.9600" Y="0.8000" />
                    <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="358.0003" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6630" />
                <PreSize X="0.8004" Y="0.1204" />
                <FileData Type="Normal" Path="account/grey_textbox_bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="password_textfield_bg" ActionTag="133177734" Tag="1794" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="55.0000" RightMargin="55.0000" TopMargin="246.5000" BottomMargin="228.5000" Scale9Enable="True" LeftEage="11" RightEage="11" TopEage="7" BottomEage="7" Scale9OriginX="11" Scale9OriginY="7" Scale9Width="157" Scale9Height="70" ctype="ImageViewObjectData">
                <Size X="441.0000" Y="65.0000" />
                <Children>
                  <AbstractNodeData Name="password_textfield" ActionTag="250793822" CallBackType="Event" Tag="1795" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="8.8200" RightMargin="8.8200" TopMargin="12.4995" BottomMargin="0.5005" TouchEnable="True" FontSize="29" IsCustomSize="True" LabelText="" PlaceHolderText="Nhập mật khẩu ..." MaxLengthText="10" PasswordEnable="True" ctype="TextFieldObjectData">
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
                <Position X="275.5000" Y="261.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4833" />
                <PreSize X="0.8004" Y="0.1204" />
                <FileData Type="Normal" Path="account/grey_textbox_bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="password_textfield_bg_0" ActionTag="655021530" Tag="1796" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="55.0000" RightMargin="55.0000" TopMargin="340.5000" BottomMargin="134.5000" Scale9Enable="True" LeftEage="11" RightEage="11" TopEage="7" BottomEage="7" Scale9OriginX="11" Scale9OriginY="7" Scale9Width="157" Scale9Height="70" ctype="ImageViewObjectData">
                <Size X="441.0000" Y="65.0000" />
                <Children>
                  <AbstractNodeData Name="reentering_password_textfield" ActionTag="-505573246" CallBackType="Event" Tag="1797" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="8.8200" RightMargin="8.8200" TopMargin="12.4995" BottomMargin="0.5005" TouchEnable="True" FontSize="29" IsCustomSize="True" LabelText="" PlaceHolderText="Nhập lại mật khẩu ..." MaxLengthText="10" PasswordEnable="True" ctype="TextFieldObjectData">
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
                <Position X="275.5000" Y="167.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.3093" />
                <PreSize X="0.8004" Y="0.1204" />
                <FileData Type="Normal" Path="account/grey_textbox_bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="register_button" ActionTag="-729483168" CallBackType="Click" CallBackName="onFinish" Tag="1798" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="177.5000" RightMargin="177.5000" TopMargin="436.5001" BottomMargin="54.4999" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="22" RightEage="22" TopEage="22" BottomEage="22" Scale9OriginX="22" Scale9OriginY="22" Scale9Width="113" Scale9Height="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="196.0000" Y="49.0000" />
                <Children>
                  <AbstractNodeData Name="register_button_title" ActionTag="-1301181576" CallBackName="onFinish" Tag="1799" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="42.0000" RightMargin="42.0000" TopMargin="9.5000" BottomMargin="9.5000" FontSize="25" LabelText="Đăng Ký" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="112.0000" Y="30.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="98.0000" Y="24.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.5714" Y="0.6122" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="78.9999" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1463" />
                <PreSize X="0.3557" Y="0.0907" />
                <TextColor A="255" R="255" G="255" B="255" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="warning_label" ActionTag="846120600" Tag="1677" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="128.5200" RightMargin="106.4800" TopMargin="499.0000" BottomMargin="19.0000" FontSize="18" LabelText="Mật khẩu nhập phải ít nhất 8 ký tự" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="316.0000" Y="22.0000" />
                <Children>
                  <AbstractNodeData Name="icon_warning" ActionTag="251995428" Tag="1678" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="-24.0000" RightMargin="322.0000" TopMargin="3.0000" BottomMargin="3.0000" LeftEage="5" RightEage="5" TopEage="5" BottomEage="5" Scale9OriginX="5" Scale9OriginY="5" Scale9Width="8" Scale9Height="6" ctype="ImageViewObjectData">
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
                <Position X="286.5200" Y="30.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="254" G="2" B="1" />
                <PrePosition X="0.5200" Y="0.0556" />
                <PreSize X="0.5735" Y="0.0407" />
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
            <PreSize X="0.5381" Y="0.7031" />
            <FileData Type="Normal" Path="account/white_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>