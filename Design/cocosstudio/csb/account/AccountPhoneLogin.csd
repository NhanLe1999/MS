<GameFile>
  <PropertyGroup Name="AccountPhoneLogin" Type="Layer" ID="61f97dbb-f51e-45e5-a35b-36a24c4850ed" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="90" Speed="1.0000" ActivedAnimationName="hide">
        <Timeline ActionTag="-487336660" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.0100" Y="0.0100">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="1.0000" Y="1.0000">
            <EasingData Type="25" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="90" X="0.0100" Y="0.0100">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="show" StartIndex="0" EndIndex="35">
          <RenderColor A="255" R="106" G="90" B="205" />
        </AnimationInfo>
        <AnimationInfo Name="hide" StartIndex="60" EndIndex="95">
          <RenderColor A="255" R="220" G="20" B="60" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="APPhoneLogin" Tag="19" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="bg" ActionTag="-2037555124" VisibleForFrame="False" Tag="69" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftEage="462" RightEage="462" TopEage="258" BottomEage="258" Scale9OriginX="-416" Scale9OriginY="-212" Scale9Width="878" Scale9Height="470" ctype="ImageViewObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Default" Path="Default/ImageFile.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="Panel_black" ActionTag="1083541531" Tag="108" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="126" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="bg_ngang" ActionTag="-487336660" Tag="84" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="162.0000" RightMargin="162.0000" TopMargin="99.0000" BottomMargin="99.0000" LeftEage="204" RightEage="204" TopEage="183" BottomEage="183" Scale9OriginX="204" Scale9OriginY="183" Scale9Width="266" Scale9Height="95" ctype="ImageViewObjectData">
            <Size X="700.0000" Y="570.0000" />
            <Children>
              <AbstractNodeData Name="title" ActionTag="573384502" Tag="85" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="81.5000" RightMargin="81.5000" TopMargin="63.6000" BottomMargin="371.4000" IsCustomSize="True" FontSize="35" LabelText="Vui lòng nhập Số điện thoại để tiếp tục" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="537.0000" Y="135.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="438.9000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.7700" />
                <PreSize X="0.7671" Y="0.2368" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="layout_name" ActionTag="-1346067729" CallBackName="onTrigger" Tag="86" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="145.0000" RightMargin="145.0000" TopMargin="255.7730" BottomMargin="234.2270" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="410.0000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="716050116" Tag="87" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="5.0000" BottomMargin="5.0000" Scale9Enable="True" LeftEage="23" RightEage="23" TopEage="26" BottomEage="26" Scale9OriginX="23" Scale9OriginY="26" Scale9Width="25" Scale9Height="21" ctype="ImageViewObjectData">
                    <Size X="410.0000" Y="70.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="205.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="0.8750" />
                    <FileData Type="Normal" Path="account/intro_license_letter.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="textfield" ActionTag="-510228533" Tag="88" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="10.0000" BottomMargin="10.0000" TouchEnable="True" FontSize="37" IsCustomSize="True" LabelText="" PlaceHolderText="" MaxLengthText="10" ctype="TextFieldObjectData">
                    <Size X="410.0000" Y="60.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="205.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="0.7500" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="icon_warning" ActionTag="627956397" VisibleForFrame="False" Tag="137" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="28.5000" RightMargin="356.5000" TopMargin="81.8000" BottomMargin="-23.8000" LeftEage="5" RightEage="5" TopEage="5" BottomEage="5" Scale9OriginX="5" Scale9OriginY="5" Scale9Width="15" Scale9Height="12" ctype="ImageViewObjectData">
                    <Size X="25.0000" Y="22.0000" />
                    <Children>
                      <AbstractNodeData Name="warning_label" ActionTag="528083796" Tag="138" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="38.0100" RightMargin="-328.0100" TopMargin="-2.4552" BottomMargin="-5.5448" IsCustomSize="True" FontSize="18" LabelText="Vui lòng nhập thông tin vào ô trống" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="315.0000" Y="30.0000" />
                        <AnchorPoint ScaleY="1.0000" />
                        <Position X="38.0100" Y="24.4552" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="254" G="2" B="1" />
                        <PrePosition X="1.5204" Y="1.1116" />
                        <PreSize X="12.6000" Y="1.3636" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Image_10" ActionTag="-1538446943" VisibleForFrame="False" Tag="139" IconVisible="False" LeftMargin="-39.3800" RightMargin="-359.6200" TopMargin="-73.5000" BottomMargin="12.5000" LeftEage="141" RightEage="141" TopEage="28" BottomEage="28" Scale9OriginX="141" Scale9OriginY="28" Scale9Width="146" Scale9Height="30" ctype="ImageViewObjectData">
                        <Size X="424.0000" Y="83.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="172.6200" Y="54.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="6.9048" Y="2.4545" />
                        <PreSize X="16.9600" Y="3.7727" />
                        <FileData Type="Normal" Path="account/red line.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="41.0000" Y="-12.8000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1000" Y="-0.1600" />
                    <PreSize X="0.0610" Y="0.2750" />
                    <FileData Type="Normal" Path="license/warningremoveview/warningremoveview_icon.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="274.2270" />
                <Scale ScaleX="1.2500" ScaleY="1.2500" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4811" />
                <PreSize X="0.5857" Y="0.1404" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_next" ActionTag="-1546273930" CallBackType="Click" CallBackName="onNext" Tag="89" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="255.0000" RightMargin="255.0000" TopMargin="441.0120" BottomMargin="62.9880" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="22" RightEage="22" TopEage="22" BottomEage="22" Scale9OriginX="22" Scale9OriginY="22" Scale9Width="113" Scale9Height="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="190.0000" Y="66.0000" />
                <Children>
                  <AbstractNodeData Name="text_next" ActionTag="1430472460" Tag="90" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="59.5000" RightMargin="59.5000" TopMargin="8.5000" BottomMargin="8.5000" FontSize="40" LabelText="Gửi" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="71.0000" Y="49.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="95.0000" Y="33.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.3737" Y="0.7424" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="95.9880" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1684" />
                <PreSize X="0.2714" Y="0.1158" />
                <TextColor A="255" R="255" G="255" B="255" />
                <PressedFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="close" ActionTag="484168869" CallBackType="Click" CallBackName="onClose" Tag="95" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="660.0000" RightMargin="-12.0000" TopMargin="-14.6000" BottomMargin="532.6000" TouchEnable="True" LeftEage="17" RightEage="17" TopEage="17" BottomEage="17" Scale9OriginX="17" Scale9OriginY="17" Scale9Width="18" Scale9Height="18" ctype="ImageViewObjectData">
                <Size X="52.0000" Y="52.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="686.0000" Y="558.6000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9800" Y="0.9800" />
                <PreSize X="0.0743" Y="0.0912" />
                <FileData Type="Normal" Path="group_promo_close.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="0.9562" ScaleY="0.9562" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.6836" Y="0.7422" />
            <FileData Type="Normal" Path="license/active_license/activelicense_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>