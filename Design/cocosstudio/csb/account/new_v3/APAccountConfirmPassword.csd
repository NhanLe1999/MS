<GameFile>
  <PropertyGroup Name="APAccountConfirmPassword" Type="Layer" ID="ae63c100-7391-4786-b900-9742530fd9a0" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="APAccountConfirmPassword" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="background" ActionTag="-933352803" Tag="207" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="-488.0000" RightMargin="-488.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="2000.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.9531" Y="1.0000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="layout" ActionTag="1758812643" Tag="33" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="title" ActionTag="2118902485" Tag="34" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="265.0000" RightMargin="265.0000" TopMargin="70.5000" BottomMargin="634.5000" FontSize="52" LabelText="Xác thực tài khoản" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="494.0000" Y="63.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="666.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="44" G="184" B="233" />
                <PrePosition X="0.5000" Y="0.8672" />
                <PreSize X="0.4824" Y="0.0820" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="desc" ActionTag="-558809979" Tag="35" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="32.0000" RightMargin="32.0000" TopMargin="138.0000" BottomMargin="430.0000" IsCustomSize="True" FontSize="32" LabelText="Vui lòng nhập mật khẩu xác thực để bảo mật tài khoản." HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="960.0000" Y="200.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="530.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.6901" />
                <PreSize X="0.9375" Y="0.2604" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="password_bg" ActionTag="496942875" Tag="36" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="204.5000" RightMargin="204.5000" TopMargin="346.0000" BottomMargin="346.0000" TouchEnable="True" LeftEage="202" RightEage="202" TopEage="25" BottomEage="25" Scale9OriginX="202" Scale9OriginY="25" Scale9Width="211" Scale9Height="26" ctype="ImageViewObjectData">
                <Size X="615.0000" Y="76.0000" />
                <Children>
                  <AbstractNodeData Name="password_textfield" ActionTag="937008487" Tag="56" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="12.3000" RightMargin="12.3000" TopMargin="16.7200" BottomMargin="13.6800" TouchEnable="True" FontSize="34" IsCustomSize="True" LabelText="" PlaceHolderText="Enter your password" MaxLengthEnable="True" MaxLengthText="32" PasswordEnable="True" ctype="TextFieldObjectData">
                    <Size X="590.4000" Y="45.6000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="12.3000" Y="36.4800" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.0200" Y="0.4800" />
                    <PreSize X="0.9600" Y="0.6000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="show_password_btn" ActionTag="192034195" CallBackType="Click" CallBackName="onShowPasswordButtonClicked" Tag="48" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="515.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="100.0000" Y="76.0000" />
                    <Children>
                      <AbstractNodeData Name="eye" ActionTag="859774362" Tag="49" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="36.0000" RightMargin="36.0000" TopMargin="29.5000" BottomMargin="29.5000" LeftEage="9" RightEage="9" TopEage="5" BottomEage="5" Scale9OriginX="9" Scale9OriginY="5" Scale9Width="10" Scale9Height="7" ctype="ImageViewObjectData">
                        <Size X="28.0000" Y="17.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="50.0000" Y="38.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.2800" Y="0.2237" />
                        <FileData Type="Normal" Path="account/new_v2/eye_enable.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="615.0000" Y="38.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="1.0000" Y="0.5000" />
                    <PreSize X="0.1626" Y="1.0000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.6006" Y="0.0990" />
                <FileData Type="Normal" Path="account/new_v3/bg_textfield_confirm_password.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_confirm" ActionTag="-2006418447" CallBackType="Click" CallBackName="onConfirm" Tag="37" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="392.0000" RightMargin="392.0000" TopMargin="562.0000" BottomMargin="144.0000" TouchEnable="True" FontSize="38" ButtonText="Xác nhận" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="210" Scale9Height="40" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="240.0000" Y="62.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="175.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2279" />
                <PreSize X="0.2344" Y="0.0807" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                <TextColor A="255" R="255" G="255" B="255" />
                <DisabledFileData Type="Normal" Path="account/new_v3/button_save_blank.png" Plist="" />
                <PressedFileData Type="Normal" Path="account/new_v3/button_save.png" Plist="" />
                <NormalFileData Type="Normal" Path="account/new_v3/button_save.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_forgot" ActionTag="-872907860" CallBackType="Click" CallBackName="onForgot" Tag="280" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="212.0000" RightMargin="512.0000" TopMargin="417.0091" BottomMargin="286.9909" TouchEnable="True" FontSize="28" ButtonText="Quên mật khẩu?" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="214" Scale9Height="12" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="300.0000" Y="64.0000" />
                <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                <Position X="512.0000" Y="318.9909" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4154" />
                <PreSize X="0.2930" Y="0.0833" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                <TextColor A="255" R="8" G="154" B="205" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="btn_back" ActionTag="-1583506299" CallBackType="Click" CallBackName="onBack" Tag="55" IconVisible="False" LeftMargin="31.0000" RightMargin="895.0000" TopMargin="19.0000" BottomMargin="651.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="68" Scale9Height="76" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="98.0000" Y="98.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="80.0000" Y="700.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.0781" Y="0.9115" />
            <PreSize X="0.0957" Y="0.1276" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Normal" Path="account/new_v3/button_back_left.png" Plist="" />
            <NormalFileData Type="Normal" Path="account/new_v3/button_back_left.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>