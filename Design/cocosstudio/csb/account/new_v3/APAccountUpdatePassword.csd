<GameFile>
  <PropertyGroup Name="APAccountUpdatePassword" Type="Layer" ID="ae63c100-7391-4786-b900-9742530fd9a0" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="layer" CustomClassName="APAccountUpdatePassword" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="background" ActionTag="-913775105" Tag="246" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="-488.0000" RightMargin="-488.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
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
          <AbstractNodeData Name="layout" ActionTag="1758812643" Tag="33" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="old_password_bg" ActionTag="496942875" Tag="36" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="204.5000" RightMargin="204.5000" TopMargin="235.0000" BottomMargin="457.0000" TouchEnable="True" LeftEage="202" RightEage="202" TopEage="25" BottomEage="25" Scale9OriginX="202" Scale9OriginY="25" Scale9Width="211" Scale9Height="26" ctype="ImageViewObjectData">
                <Size X="615.0000" Y="76.0000" />
                <Children>
                  <AbstractNodeData Name="old_password_textField" ActionTag="937008487" Tag="56" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="12.3000" RightMargin="12.3000" TopMargin="16.7200" BottomMargin="13.6800" TouchEnable="True" FontSize="34" IsCustomSize="True" LabelText="" PlaceHolderText="old password" MaxLengthEnable="True" MaxLengthText="32" PasswordEnable="True" ctype="TextFieldObjectData">
                    <Size X="590.4000" Y="45.6000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="12.3000" Y="36.4800" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.0200" Y="0.4800" />
                    <PreSize X="0.9600" Y="0.6000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="show_old_password_btn" ActionTag="1083961510" CallBackType="Click" CallBackName="onShowPasswordButtonClicked" Tag="179" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="515.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="100.0000" Y="76.0000" />
                    <Children>
                      <AbstractNodeData Name="eye" ActionTag="-994735389" Tag="180" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="36.0000" RightMargin="36.0000" TopMargin="29.5000" BottomMargin="29.5000" LeftEage="9" RightEage="9" TopEage="5" BottomEage="5" Scale9OriginX="9" Scale9OriginY="5" Scale9Width="10" Scale9Height="7" ctype="ImageViewObjectData">
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
                <Position X="512.0000" Y="495.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6445" />
                <PreSize X="0.6006" Y="0.0990" />
                <FileData Type="Normal" Path="account/new_v3/bg_textfield_confirm_password.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="password_bg" ActionTag="843736996" Tag="111" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="204.5000" RightMargin="204.5000" TopMargin="346.0000" BottomMargin="346.0000" LeftEage="202" RightEage="202" TopEage="25" BottomEage="25" Scale9OriginX="202" Scale9OriginY="25" Scale9Width="211" Scale9Height="26" ctype="ImageViewObjectData">
                <Size X="615.0000" Y="76.0000" />
                <Children>
                  <AbstractNodeData Name="password_textField" ActionTag="348806060" Tag="112" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="12.3000" RightMargin="12.3000" TopMargin="16.7200" BottomMargin="13.6800" TouchEnable="True" FontSize="34" IsCustomSize="True" LabelText="" PlaceHolderText="new password" MaxLengthEnable="True" MaxLengthText="32" PasswordEnable="True" ctype="TextFieldObjectData">
                    <Size X="590.4000" Y="45.6000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="12.3000" Y="36.4800" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.0200" Y="0.4800" />
                    <PreSize X="0.9600" Y="0.6000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="show_password_btn" ActionTag="-1005478475" CallBackType="Click" CallBackName="onShowPasswordButtonClicked" Tag="181" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="515.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="100.0000" Y="76.0000" />
                    <Children>
                      <AbstractNodeData Name="eye" ActionTag="2109868711" Tag="182" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="36.0000" RightMargin="36.0000" TopMargin="29.5000" BottomMargin="29.5000" LeftEage="9" RightEage="9" TopEage="5" BottomEage="5" Scale9OriginX="9" Scale9OriginY="5" Scale9Width="10" Scale9Height="7" ctype="ImageViewObjectData">
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
              <AbstractNodeData Name="re_password_bg" ActionTag="-1400841911" Tag="113" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="204.5000" RightMargin="204.5000" TopMargin="457.0000" BottomMargin="235.0000" LeftEage="202" RightEage="202" TopEage="25" BottomEage="25" Scale9OriginX="202" Scale9OriginY="25" Scale9Width="211" Scale9Height="26" ctype="ImageViewObjectData">
                <Size X="615.0000" Y="76.0000" />
                <Children>
                  <AbstractNodeData Name="re_password_textField" ActionTag="-1970721787" Tag="114" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="12.3000" RightMargin="12.3000" TopMargin="16.7200" BottomMargin="13.6800" TouchEnable="True" FontSize="34" IsCustomSize="True" LabelText="" PlaceHolderText="re-new password" MaxLengthEnable="True" MaxLengthText="32" PasswordEnable="True" ctype="TextFieldObjectData">
                    <Size X="590.4000" Y="45.6000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="12.3000" Y="36.4800" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.0200" Y="0.4800" />
                    <PreSize X="0.9600" Y="0.6000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="show_re_password_btn" ActionTag="-1951300648" CallBackType="Click" CallBackName="onShowPasswordButtonClicked" Tag="183" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="515.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="100.0000" Y="76.0000" />
                    <Children>
                      <AbstractNodeData Name="eye" ActionTag="1101007728" Tag="184" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="36.0000" RightMargin="36.0000" TopMargin="29.5000" BottomMargin="29.5000" LeftEage="9" RightEage="9" TopEage="5" BottomEage="5" Scale9OriginX="9" Scale9OriginY="5" Scale9Width="10" Scale9Height="7" ctype="ImageViewObjectData">
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
                <Position X="512.0000" Y="273.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.3555" />
                <PreSize X="0.6006" Y="0.0990" />
                <FileData Type="Normal" Path="account/new_v3/bg_textfield_confirm_password.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_update" ActionTag="-2006418447" CallBackType="Click" CallBackName="onUpdate" Tag="37" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="392.0000" RightMargin="392.0000" TopMargin="617.0000" BottomMargin="89.0000" TouchEnable="True" FontSize="40" ButtonText="Lưu" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="210" Scale9Height="40" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="240.0000" Y="62.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="120.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.2000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1563" />
                <PreSize X="0.2344" Y="0.0807" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                <TextColor A="255" R="255" G="255" B="255" />
                <DisabledFileData Type="Normal" Path="account/new_v3/button_save_blank.png" Plist="" />
                <PressedFileData Type="Normal" Path="account/new_v3/button_save.png" Plist="" />
                <NormalFileData Type="Normal" Path="account/new_v3/button_save.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="title" ActionTag="2118902485" Tag="34" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="257.5000" RightMargin="257.5000" TopMargin="70.5000" BottomMargin="634.5000" FontSize="52" LabelText="Cập nhật mật khẩu" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="509.0000" Y="63.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="666.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="44" G="184" B="233" />
                <PrePosition X="0.5000" Y="0.8672" />
                <PreSize X="0.4971" Y="0.0820" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
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