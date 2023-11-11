<GameFile>
  <PropertyGroup Name="ForgotLicense" Type="Layer" ID="5dd44831-8af2-4dd4-b01e-6246f26e05b4" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="30" Speed="1.0000" ActivedAnimationName="preloader">
        <Timeline ActionTag="6866933" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="180.0000" Y="180.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="preloader" StartIndex="0" EndIndex="30">
          <RenderColor A="255" R="188" G="143" B="143" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="MSForgotLicensePopup" Tag="113" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="Panel_1" ActionTag="-1003080675" Alpha="204" CallBackType="Click" CallBackName="onClose" Tag="163" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TopMargin="1.3176" BottomMargin="-1.3176" TouchEnable="True" ClipAble="False" BackColorAlpha="229" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint />
            <Position Y="-1.3176" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition Y="-0.0017" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="bg" ActionTag="-1825084475" Tag="114" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="137.0000" RightMargin="137.0000" TopMargin="184.0000" BottomMargin="184.0000" Scale9Enable="True" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="1" Scale9Height="1" ctype="ImageViewObjectData">
            <Size X="750.0000" Y="400.0000" />
            <Children>
              <AbstractNodeData Name="Image_1" ActionTag="-778253953" Tag="265" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="50.0000" RightMargin="50.0000" TopMargin="135.0000" BottomMargin="175.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="51" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
                <Size X="650.0000" Y="90.0000" />
                <Children>
                  <AbstractNodeData Name="textfield" Visible="False" ActionTag="1594625607" CallBackType="Event" CallBackName="onTextfieldChanged" Tag="121" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="15.0000" BottomMargin="15.0000" TouchEnable="True" FontSize="25" IsCustomSize="True" LabelText="" PlaceHolderText="" MaxLengthText="10" ctype="TextFieldObjectData">
                    <Size X="650.0000" Y="60.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="325.0000" Y="45.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="52" G="197" B="237" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="0.6667" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="375.0000" Y="220.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5500" />
                <PreSize X="0.8667" Y="0.2250" />
                <SingleColor A="255" R="0" G="0" B="0" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="forgot.license.ask" ActionTag="1433732079" Tag="122" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="TopEdge" LeftMargin="50.0000" RightMargin="50.0000" TopMargin="20.0000" BottomMargin="300.0000" IsCustomSize="True" FontSize="26" LabelText="Vui lòng nhập email hoặc số điện thoại bạn đã đăng ký mua chương trinh" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="650.0000" Y="80.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="375.0000" Y="340.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.8500" />
                <PreSize X="0.8667" Y="0.2000" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_2" ActionTag="6866933" Tag="123" RotationSkewX="180.0000" RotationSkewY="180.0000" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="350.0000" RightMargin="350.0000" TopMargin="315.0000" BottomMargin="35.0000" LeftEage="13" RightEage="13" TopEage="13" BottomEage="13" Scale9OriginX="13" Scale9OriginY="13" Scale9Width="24" Scale9Height="24" ctype="ImageViewObjectData">
                <Size X="50.0000" Y="50.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="375.0000" Y="60.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1500" />
                <PreSize X="0.0667" Y="0.1250" />
                <FileData Type="Normal" Path="mjstory/indicator.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_7" ActionTag="1377709391" CallBackType="Click" CallBackName="onClose" Tag="162" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="726.5000" RightMargin="-23.5000" TopMargin="-23.5000" BottomMargin="376.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="17" Scale9Height="25" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="47.0000" Y="47.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="750.0000" Y="400.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="1.0000" Y="1.0000" />
                <PreSize X="0.0627" Y="0.1175" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/popup/popup_ratebuy_btclose.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="panel_warning" ActionTag="-958340348" Tag="311" IconVisible="False" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="55.0000" RightMargin="20.0000" TopMargin="215.0000" BottomMargin="135.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="675.0000" Y="50.0000" />
                <Children>
                  <AbstractNodeData Name="img_warning" ActionTag="-1506655547" Tag="312" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" RightMargin="639.0000" TopMargin="18.0000" LeftEage="5" RightEage="5" TopEage="5" BottomEage="5" Scale9OriginX="5" Scale9OriginY="5" Scale9Width="8" Scale9Height="6" ctype="ImageViewObjectData">
                    <Size X="36.0000" Y="32.0000" />
                    <AnchorPoint />
                    <Position />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="0.0533" Y="0.6400" />
                    <FileData Type="Normal" Path="license/active_license/activelicense_iconwarning.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="txt_warning" ActionTag="-361431721" Tag="313" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="50.0000" RightMargin="281.0000" TopMargin="23.0000" FontSize="22" LabelText="Vui lòng nhập lại mã kích hoạt" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="344.0000" Y="27.0000" />
                    <AnchorPoint />
                    <Position X="50.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="0" B="0" />
                    <PrePosition X="0.0741" />
                    <PreSize X="0.5096" Y="0.5400" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="0.5000" />
                <Position X="55.0000" Y="160.0000" />
                <Scale ScaleX="0.9441" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0733" Y="0.4000" />
                <PreSize X="0.9000" Y="0.1250" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="button" ActionTag="541856186" CallBackType="Click" CallBackName="onOK" Tag="124" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="305.0000" RightMargin="305.0000" TopMargin="310.0000" BottomMargin="30.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="6" RightEage="6" TopEage="6" BottomEage="6" Scale9OriginX="6" Scale9OriginY="6" Scale9Width="8" Scale9Height="8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="140.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="forgot.license.action" ActionTag="1150732815" Tag="125" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="49.5000" RightMargin="49.5000" TopMargin="15.5000" BottomMargin="15.5000" FontSize="24" LabelText="OK" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="41.0000" Y="29.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="70.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.2929" Y="0.4833" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="375.0000" Y="60.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1500" />
                <PreSize X="0.1867" Y="0.1500" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/parent/parent_notification_item_open_url.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.7324" Y="0.5208" />
            <FileData Type="Normal" Path="mjstory/parent/parent_license_background.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>