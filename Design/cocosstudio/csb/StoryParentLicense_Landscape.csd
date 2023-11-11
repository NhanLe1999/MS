<GameFile>
  <PropertyGroup Name="StoryParentLicense_Landscape" Type="Layer" ID="0109156e-b1ca-4a3b-8fab-5ac0e3a4cca9" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="30" Speed="1.0000" ActivedAnimationName="preloader">
        <Timeline ActionTag="-413700500" Property="RotationSkew">
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
          <RenderColor A="255" R="173" G="255" B="47" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="StoryParentLicense_Landscape" Tag="22" ctype="GameLayerObjectData">
        <Size X="700.0000" Y="628.0000" />
        <Children>
          <AbstractNodeData Name="button_deactivate" ActionTag="-1877061036" VisibleForFrame="False" CallBackType="Click" CallBackName="onDeactivate" Tag="97" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="BottomEdge" LeftMargin="500.0000" TopMargin="608.0000" BottomMargin="-20.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="-15" Scale9OriginY="-6" Scale9Width="30" Scale9Height="12" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="200.0000" Y="40.0000" />
            <Children>
              <AbstractNodeData Name="text_deactive_device" ActionTag="-1413376996" Tag="96" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="17.5000" RightMargin="17.5000" TopMargin="11.0000" BottomMargin="11.0000" FontSize="15" LabelText="Hủy kích hoạt thiết bị" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="165.0000" Y="18.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="100.0000" Y="20.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="52" G="197" B="237" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.8250" Y="0.4500" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Italic.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Panel_4" ActionTag="-889069778" Tag="98" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="20.0000" RightMargin="20.0000" TopMargin="31.0000" BottomMargin="8.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="160.0000" Y="1.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="100.0000" Y="8.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2000" />
                <PreSize X="0.8000" Y="0.0250" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="600.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.8571" />
            <PreSize X="0.2857" Y="0.0637" />
            <TextColor A="255" R="65" G="65" B="70" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="license_bg" ActionTag="-73561065" Tag="1449" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="50.0000" RightMargin="50.0000" TopMargin="101.5000" BottomMargin="101.5000" Scale9Enable="True" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="1" Scale9Height="1" ctype="ImageViewObjectData">
            <Size X="600.0000" Y="425.0000" />
            <Children>
              <AbstractNodeData Name="panel_has_license" ActionTag="-622803612" Tag="1450" IconVisible="False" LeftMargin="-1.0000" RightMargin="1.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="600.0000" Y="425.0000" />
                <Children>
                  <AbstractNodeData Name="parent.license.info" ActionTag="1915072051" Tag="1451" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="TopEdge" LeftMargin="139.5000" RightMargin="139.5000" TopMargin="39.5000" BottomMargin="331.5000" FontSize="22" LabelText="Thiết bị đang được kích hoạt &#xA;" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="321.0000" Y="54.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="300.0000" Y="358.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5000" Y="0.8435" />
                    <PreSize X="0.5350" Y="0.1271" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_license" ActionTag="-712624080" VisibleForFrame="False" Tag="1452" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="TopEdge" LeftMargin="233.0000" RightMargin="233.0000" TopMargin="67.5000" BottomMargin="313.5000" FontSize="36" LabelText="AZU47" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="134.0000" Y="44.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="300.0000" Y="335.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="52" G="197" B="237" />
                    <PrePosition X="0.5000" Y="0.7894" />
                    <PreSize X="0.2233" Y="0.1035" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Black.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="parent.license.expire" ActionTag="707786102" Tag="1453" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="TopEdge" LeftMargin="177.9640" RightMargin="177.0360" TopMargin="93.5556" BottomMargin="307.4444" FontSize="20" LabelText="Hạn sử dụng: 01/01/2047" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="245.0000" Y="24.0000" />
                    <AnchorPoint ScaleX="0.5128" ScaleY="0.4815" />
                    <Position X="303.6000" Y="319.0004" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5060" Y="0.7506" />
                    <PreSize X="0.4083" Y="0.0565" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_3" ActionTag="-554375597" Tag="1454" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="100.0000" RightMargin="100.0000" TopMargin="132.5000" BottomMargin="257.5000" LeftEage="165" RightEage="165" TopEage="11" BottomEage="11" Scale9OriginX="165" Scale9OriginY="11" Scale9Width="170" Scale9Height="13" ctype="ImageViewObjectData">
                    <Size X="400.0000" Y="35.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="300.0000" Y="275.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.6471" />
                    <PreSize X="0.6667" Y="0.0824" />
                    <FileData Type="Normal" Path="mjstory/pay/pay_arrow.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="-1.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="-0.0017" />
                <PreSize X="1.0000" Y="1.0000" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_1" ActionTag="1080059068" Tag="1455" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="25.0000" RightMargin="25.0000" TopMargin="278.0000" BottomMargin="87.0000" Scale9Enable="True" LeftEage="12" RightEage="12" TopEage="12" BottomEage="12" Scale9OriginX="12" Scale9OriginY="12" Scale9Width="1" Scale9Height="1" ctype="ImageViewObjectData">
                <Size X="550.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="textfield_licensekey" ActionTag="572113301" CallBackType="Event" CallBackName="onTextfieldChanged" Tag="1456" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" FontSize="36" IsCustomSize="True" LabelText="" PlaceHolderText="License Key" MaxLengthText="10" ctype="TextFieldObjectData">
                    <Size X="550.0000" Y="60.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="275.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="52" G="197" B="237" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="300.0000" Y="117.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2753" />
                <PreSize X="0.9167" Y="0.1412" />
                <FileData Type="Normal" Path="mjstory/parent/parent_license_textfield_background.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="parent.license.ask" ActionTag="-1509631254" Tag="1457" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="45.0000" RightMargin="45.0000" TopMargin="189.0000" BottomMargin="182.0000" FontSize="22" LabelText="Nếu bạn đã đăng ký mua và có mã kích hoạt,&#xA;vui lòng nhập tại đây" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="510.0000" Y="54.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="300.0000" Y="209.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.4918" />
                <PreSize X="0.8500" Y="0.1271" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_2" ActionTag="-413700500" Tag="1458" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="275.0000" RightMargin="275.0000" TopMargin="360.0000" BottomMargin="15.0000" LeftEage="13" RightEage="13" TopEage="13" BottomEage="13" Scale9OriginX="13" Scale9OriginY="13" Scale9Width="24" Scale9Height="24" ctype="ImageViewObjectData">
                <Size X="50.0000" Y="50.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="300.0000" Y="40.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.0941" />
                <PreSize X="0.0833" Y="0.1176" />
                <FileData Type="Normal" Path="mjstory/indicator.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="button" ActionTag="515734422" CallBackType="Click" CallBackName="onOK" Tag="1459" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="250.0000" RightMargin="250.0000" TopMargin="362.5000" BottomMargin="17.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="6" RightEage="6" TopEage="6" BottomEage="6" Scale9OriginX="6" Scale9OriginY="6" Scale9Width="8" Scale9Height="8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="100.0000" Y="45.0000" />
                <Children>
                  <AbstractNodeData Name="Text_2" ActionTag="968087064" Tag="1460" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="29.5000" RightMargin="29.5000" TopMargin="8.0000" BottomMargin="8.0000" FontSize="24" LabelText="OK" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="41.0000" Y="29.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="50.0000" Y="22.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.4100" Y="0.6444" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="300.0000" Y="40.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.0941" />
                <PreSize X="0.1667" Y="0.1059" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/parent/parent_notification_item_open_url.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="parent.license.note" ActionTag="144453152" VisibleForFrame="False" Alpha="153" Tag="146" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="197.0000" RightMargin="197.0000" TopMargin="261.0000" BottomMargin="144.0000" FontSize="16" LabelText="Mã kích hoạt gồm 5 ký tự" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="206.0000" Y="20.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="300.0000" Y="154.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.3624" />
                <PreSize X="0.3433" Y="0.0471" />
                <FontResource Type="Normal" Path="fonts/Montserrat-LightItalic.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_forgot_license" ActionTag="-839407291" CallBackType="Click" CallBackName="onForgotLicense" Tag="110" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="50.0000" RightMargin="350.0000" TopMargin="425.7669" BottomMargin="-40.7669" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="-15" Scale9OriginY="-6" Scale9Width="30" Scale9Height="12" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="200.0000" Y="40.0000" />
                <Children>
                  <AbstractNodeData Name="text_forgot_license" ActionTag="763970206" Tag="111" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="21.0000" RightMargin="21.0000" TopMargin="11.0000" BottomMargin="11.0000" FontSize="15" LabelText="Quên mã kích hoạt?" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="158.0000" Y="18.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="100.0000" Y="20.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="52" G="197" B="237" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.7900" Y="0.4500" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Italic.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Panel_4" ActionTag="-1712876860" Tag="112" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="20.0000" RightMargin="20.0000" TopMargin="31.0000" BottomMargin="8.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="160.0000" Y="1.0000" />
                    <AnchorPoint ScaleX="0.5000" />
                    <Position X="100.0000" Y="8.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.2000" />
                    <PreSize X="0.8000" Y="0.0250" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="150.0000" Y="-20.7669" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2500" Y="-0.0489" />
                <PreSize X="0.3333" Y="0.0941" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_subscribe" ActionTag="548895142" CallBackType="Click" CallBackName="onSubScribe" Tag="87" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="327.3600" RightMargin="62.6400" TopMargin="425.7669" BottomMargin="-40.7669" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="-15" Scale9OriginY="-6" Scale9Width="30" Scale9Height="12" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="210.0000" Y="40.0000" />
                <Children>
                  <AbstractNodeData Name="text_subscribe" ActionTag="-1914548699" Tag="88" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TopMargin="11.0000" BottomMargin="11.0000" FontSize="15" LabelText="Đăng ký mua mã kích hoạt" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="210.0000" Y="18.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="105.0000" Y="20.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="52" G="197" B="237" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="0.4500" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Italic.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="panel_subscribe" ActionTag="25300146" Tag="89" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="5.0000" RightMargin="5.0000" TopMargin="31.0000" BottomMargin="8.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="200.0000" Y="1.0000" />
                    <AnchorPoint ScaleX="0.5000" />
                    <Position X="105.0000" Y="8.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.2000" />
                    <PreSize X="0.9524" Y="0.0250" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="432.3600" Y="-20.7669" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7206" Y="-0.0489" />
                <PreSize X="0.3500" Y="0.0941" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="350.0000" Y="314.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.8571" Y="0.6768" />
            <FileData Type="Normal" Path="mjstory/parent/parent_license_background.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>