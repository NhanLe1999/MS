<GameFile>
  <PropertyGroup Name="APEnterNewPasswordPopup" Type="Layer" ID="eec4a6ec-c418-4ee4-a3ef-6154662572e2" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="APEnterNewPasswordPopup" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="popup_bg" ActionTag="-394346387" Tag="360" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="236.5000" RightMargin="236.5000" TopMargin="159.0000" BottomMargin="159.0000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="64" Scale9Height="52" ctype="ImageViewObjectData">
            <Size X="551.0000" Y="450.0000" />
            <Children>
              <AbstractNodeData Name="exit_button" ActionTag="-1758600910" CallBackType="Click" CallBackName="onCancel" Tag="2006" IconVisible="False" LeftMargin="501.2911" RightMargin="-10.2911" TopMargin="-6.1408" BottomMargin="396.1408" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="60.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="-1458779650" Tag="3295" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="12.5000" RightMargin="12.5000" TopMargin="12.5000" BottomMargin="12.5000" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="21" Scale9Height="21" ctype="ImageViewObjectData">
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
                <Position X="531.2911" Y="426.1408" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9642" Y="0.9470" />
                <PreSize X="0.1089" Y="0.1333" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="title" ActionTag="1717790470" Tag="362" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="28.0000" RightMargin="28.0000" TopMargin="30.0001" BottomMargin="317.9999" FontSize="28" LabelText="Ba mẹ đã xác thực tài khoản thành&#xA; công, vui lòng đặt lại mật khẩu để &#xA;đăng nhập ứng dụng" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="495.0000" Y="102.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="368.9999" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.8200" />
                <PreSize X="0.8984" Y="0.2267" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="password_textfield_bg" ActionTag="1314826383" Tag="461" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="55.0000" RightMargin="55.0000" TopMargin="143.5000" BottomMargin="241.5000" Scale9Enable="True" LeftEage="11" RightEage="11" TopEage="7" BottomEage="7" Scale9OriginX="11" Scale9OriginY="7" Scale9Width="157" Scale9Height="70" ctype="ImageViewObjectData">
                <Size X="441.0000" Y="65.0000" />
                <Children>
                  <AbstractNodeData Name="password_textfield" ActionTag="2075043200" CallBackType="Event" Tag="462" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="8.8200" RightMargin="8.8200" TopMargin="12.4995" BottomMargin="0.5005" TouchEnable="True" FontSize="29" IsCustomSize="True" LabelText="" PlaceHolderText="Nhập mật khẩu ..." MaxLengthText="10" PasswordEnable="True" ctype="TextFieldObjectData">
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
                <PrePosition X="0.5000" Y="0.6089" />
                <PreSize X="0.8004" Y="0.1444" />
                <FileData Type="Normal" Path="account/grey_textbox_bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="password_textfield_bg_0" ActionTag="-1418085552" Tag="1181" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="55.0000" RightMargin="55.0000" TopMargin="237.5000" BottomMargin="147.5000" Scale9Enable="True" LeftEage="11" RightEage="11" TopEage="7" BottomEage="7" Scale9OriginX="11" Scale9OriginY="7" Scale9Width="157" Scale9Height="70" ctype="ImageViewObjectData">
                <Size X="441.0000" Y="65.0000" />
                <Children>
                  <AbstractNodeData Name="reentering_password_textfield" ActionTag="466439528" CallBackType="Event" Tag="1182" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="8.8200" RightMargin="8.8200" TopMargin="12.4995" BottomMargin="0.5005" TouchEnable="True" FontSize="29" IsCustomSize="True" LabelText="" PlaceHolderText="Nhập lại mật khẩu ..." MaxLengthText="10" PasswordEnable="True" ctype="TextFieldObjectData">
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
                <PrePosition X="0.5000" Y="0.4000" />
                <PreSize X="0.8004" Y="0.1444" />
                <FileData Type="Normal" Path="account/grey_textbox_bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="finish_button" ActionTag="-1114202442" CallBackType="Click" CallBackName="onFinish" Tag="647" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="177.5000" RightMargin="177.5000" TopMargin="345.5000" BottomMargin="55.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="22" RightEage="22" TopEage="22" BottomEage="22" Scale9OriginX="22" Scale9OriginY="22" Scale9Width="113" Scale9Height="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="196.0000" Y="49.0000" />
                <Children>
                  <AbstractNodeData Name="finish_button_title" ActionTag="714587029" Tag="648" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="19.5000" RightMargin="19.5000" TopMargin="9.5000" BottomMargin="9.5000" FontSize="25" LabelText="Hoàn Thành" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
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
                <PrePosition X="0.5000" Y="0.1778" />
                <PreSize X="0.3557" Y="0.1089" />
                <TextColor A="255" R="255" G="255" B="255" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="warning_label" ActionTag="-1559361192" Tag="1523" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="128.5200" RightMargin="106.4800" TopMargin="414.0000" BottomMargin="14.0000" FontSize="18" LabelText="Mật khẩu nhập phải ít nhất 8 ký tự" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="316.0000" Y="22.0000" />
                <Children>
                  <AbstractNodeData Name="icon_warning" ActionTag="734202322" Tag="1524" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="-24.0000" RightMargin="322.0000" TopMargin="3.0000" BottomMargin="3.0000" LeftEage="5" RightEage="5" TopEage="5" BottomEage="5" Scale9OriginX="5" Scale9OriginY="5" Scale9Width="8" Scale9Height="6" ctype="ImageViewObjectData">
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
                <PrePosition X="0.5200" Y="0.0556" />
                <PreSize X="0.5735" Y="0.0489" />
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
            <PreSize X="0.5381" Y="0.5859" />
            <FileData Type="Normal" Path="account/white_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>