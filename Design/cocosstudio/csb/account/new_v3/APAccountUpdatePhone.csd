<GameFile>
  <PropertyGroup Name="APAccountUpdatePhone" Type="Layer" ID="ae63c100-7391-4786-b900-9742530fd9a0" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="APAccountUpdatePhone" Tag="31" ctype="GameLayerObjectData">
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
          <AbstractNodeData Name="layout" ActionTag="1758812643" Tag="33" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="title" ActionTag="2118902485" Tag="34" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="215.0000" RightMargin="215.0000" TopMargin="70.5000" BottomMargin="634.5000" FontSize="52" LabelText="Cập nhật số điện thoại" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="594.0000" Y="63.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="666.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="44" G="184" B="233" />
                <PrePosition X="0.5000" Y="0.8672" />
                <PreSize X="0.5801" Y="0.0820" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="desc" ActionTag="-558809979" Tag="35" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="132.0000" RightMargin="132.0000" TopMargin="133.0000" BottomMargin="385.0000" IsCustomSize="True" FontSize="32" LabelText="Vui lòng cập nhật số điện thoại để Monkey hỗ trợ tốt nhất cho quyền lợi của bạn.&#xA;Đồng thời có thêm một phương thức đăng nhập mới." HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="760.0000" Y="250.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="510.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.6641" />
                <PreSize X="0.7422" Y="0.3255" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="phone_bg" ActionTag="496942875" Tag="36" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="204.5000" RightMargin="204.5000" TopMargin="400.0000" BottomMargin="292.0000" TouchEnable="True" LeftEage="202" RightEage="202" TopEage="25" BottomEage="25" Scale9OriginX="202" Scale9OriginY="25" Scale9Width="211" Scale9Height="26" ctype="ImageViewObjectData">
                <Size X="615.0000" Y="76.0000" />
                <Children>
                  <AbstractNodeData Name="btn_country" ActionTag="-1990350148" CallBackType="Click" CallBackName="onCountryCodeClicked" Tag="1356" IconVisible="False" PositionPercentYEnabled="True" RightMargin="455.0000" TopMargin="-2.0000" BottomMargin="-2.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="-15" Scale9OriginY="-11" Scale9Width="30" Scale9Height="22" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="160.0000" Y="80.0000" />
                    <Children>
                      <AbstractNodeData Name="countrycode_text" ActionTag="474116467" Tag="1354" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="-38.0000" RightMargin="98.0000" TopMargin="10.0000" BottomMargin="10.0000" IsCustomSize="True" FontSize="30" LabelText="VN" HorizontalAlignmentType="HT_Right" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="100.0000" Y="60.0000" />
                        <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                        <Position X="62.0000" Y="40.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.3875" Y="0.5000" />
                        <PreSize X="0.6250" Y="0.7500" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Text_1_0" ActionTag="272823558" Tag="1550" RotationSkewX="90.0000" RotationSkewY="90.0000" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="65.5000" RightMargin="81.5000" TopMargin="29.3000" BottomMargin="27.7000" FontSize="20" LabelText="&gt;" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="13.0000" Y="23.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="72.0000" Y="39.2000" />
                        <Scale ScaleX="1.0000" ScaleY="1.3000" />
                        <CColor A="255" R="63" G="63" B="63" />
                        <PrePosition X="0.4500" Y="0.4900" />
                        <PreSize X="0.0812" Y="0.2875" />
                        <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleY="0.5000" />
                    <Position Y="38.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition Y="0.5000" />
                    <PreSize X="0.2602" Y="1.0526" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="numbercode_text" ActionTag="-149505032" Tag="1355" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="86.0000" RightMargin="409.0000" TopMargin="8.0000" BottomMargin="8.0000" IsCustomSize="True" FontSize="30" LabelText="+84" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="120.0000" Y="60.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="86.0000" Y="38.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.1398" Y="0.5000" />
                    <PreSize X="0.1951" Y="0.7895" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="phone_textfield" ActionTag="937008487" Tag="56" IconVisible="False" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="185.0000" RightMargin="10.0000" TopMargin="19.3800" BottomMargin="11.0200" TouchEnable="True" FontSize="30" IsCustomSize="True" LabelText="" PlaceHolderText="Nhập số điện thoại của bạn" MaxLengthEnable="True" MaxLengthText="16" ctype="TextFieldObjectData">
                    <Size X="420.0000" Y="45.6000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="185.0000" Y="33.8200" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.3008" Y="0.4450" />
                    <PreSize X="0.6829" Y="0.6000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="330.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4297" />
                <PreSize X="0.6006" Y="0.0990" />
                <FileData Type="Normal" Path="account/new_v3/bg_textfield_confirm_password.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_update" ActionTag="-2006418447" CallBackType="Click" CallBackName="onUpdate" Tag="37" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="392.0000" RightMargin="392.0000" TopMargin="555.6505" BottomMargin="150.3495" TouchEnable="True" FontSize="38" ButtonText="Lưu" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="210" Scale9Height="40" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="240.0000" Y="62.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="181.3495" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2361" />
                <PreSize X="0.2344" Y="0.0807" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                <TextColor A="255" R="255" G="255" B="255" />
                <DisabledFileData Type="Normal" Path="account/new_v3/button_save_blank.png" Plist="" />
                <PressedFileData Type="Normal" Path="account/new_v3/button_save.png" Plist="" />
                <NormalFileData Type="Normal" Path="account/new_v3/button_save.png" Plist="" />
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