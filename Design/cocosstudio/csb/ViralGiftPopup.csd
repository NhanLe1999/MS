<GameFile>
  <PropertyGroup Name="ViralGiftPopup" Type="Layer" ID="4b06ef18-14e5-4398-9e0a-e4bdc17ab25b" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="85" Speed="1.0000">
        <Timeline ActionTag="384656886" Property="Scale">
          <ScaleFrame FrameIndex="5" X="0.0100" Y="0.0100">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="35" X="1.0000" Y="1.0000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="55" X="1.0000" Y="1.0000">
            <EasingData Type="25" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="85" X="0.0100" Y="0.0100">
            <EasingData Type="25" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="show" StartIndex="5" EndIndex="40">
          <RenderColor A="255" R="255" G="240" B="245" />
        </AnimationInfo>
        <AnimationInfo Name="hide" StartIndex="55" EndIndex="90">
          <RenderColor A="255" R="106" G="90" B="205" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="layer" CustomClassName="ViralGiftPopup" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="layout" ActionTag="-1756766247" Tag="169" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="153" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="bg_ngang" ActionTag="384656886" Tag="120" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="207.0000" RightMargin="207.0000" TopMargin="99.0000" BottomMargin="99.0000" LeftEage="152" RightEage="152" TopEage="109" BottomEage="109" Scale9OriginX="152" Scale9OriginY="109" Scale9Width="313" Scale9Height="359" ctype="ImageViewObjectData">
            <Size X="610.0000" Y="570.0000" />
            <Children>
              <AbstractNodeData Name="monkey" ActionTag="-918091593" Tag="122" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="370.0000" RightMargin="4.0000" TopMargin="2.1000" BottomMargin="252.9000" LeftEage="77" RightEage="77" TopEage="33" BottomEage="33" Scale9OriginX="77" Scale9OriginY="33" Scale9Width="82" Scale9Height="249" ctype="ImageViewObjectData">
                <Size X="236.0000" Y="315.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="488.0000" Y="410.4000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8000" Y="0.7200" />
                <PreSize X="0.3869" Y="0.5526" />
                <FileData Type="Normal" Path="mjstory/popup/viral_popup_monkey.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="cloud" ActionTag="-621530276" Tag="123" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="45.0000" RightMargin="228.0000" TopMargin="64.4000" BottomMargin="269.6000" LeftEage="111" RightEage="111" TopEage="77" BottomEage="77" Scale9OriginX="111" Scale9OriginY="77" Scale9Width="115" Scale9Height="82" ctype="ImageViewObjectData">
                <Size X="337.0000" Y="236.0000" />
                <Children>
                  <AbstractNodeData Name="title" ActionTag="-699962796" Tag="124" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="39.3790" RightMargin="61.6210" TopMargin="86.8000" BottomMargin="63.2000" FontSize="35" LabelText="CHÚC MỪNG&#xA;BẠN!" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="236.0000" Y="86.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="157.3790" Y="106.2000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="85" G="151" B="223" />
                    <PrePosition X="0.4670" Y="0.4500" />
                    <PreSize X="0.7003" Y="0.3644" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="213.5000" Y="387.6000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3500" Y="0.6800" />
                <PreSize X="0.5525" Y="0.4140" />
                <FileData Type="Normal" Path="mjstory/popup/viral_popup_cloud.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="content" ActionTag="-260603777" Tag="125" IconVisible="False" PositionPercentXEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="15.2500" RightMargin="15.2500" TopMargin="300.0000" BottomMargin="150.0000" IsCustomSize="True" FontSize="36" LabelText="Bạn có 7 ngày học&#xA;miễn phí chương trình" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="579.5000" Y="120.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="305.0000" Y="210.0000" />
                <Scale ScaleX="0.9500" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.3684" />
                <PreSize X="0.9500" Y="0.2105" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btfree" ActionTag="1006248548" CallBackType="Click" CallBackName="onOK" Tag="126" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="185.5000" RightMargin="185.5000" TopMargin="442.0000" BottomMargin="43.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="209" Scale9Height="63" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="239.0000" Y="85.0000" />
                <Children>
                  <AbstractNodeData Name="titlebtn" ActionTag="-666237382" Tag="128" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="83.1100" RightMargin="87.8900" TopMargin="13.7500" BottomMargin="22.2500" FontSize="40" LabelText="OK" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="68.0000" Y="49.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="117.1100" Y="46.7500" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4900" Y="0.5500" />
                    <PreSize X="0.2845" Y="0.5765" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="77" G="77" B="77" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="305.0000" Y="85.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1500" />
                <PreSize X="0.3918" Y="0.1491" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/popup/viral_popup_button.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="0.5243" ScaleY="0.5243" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.5957" Y="0.7422" />
            <FileData Type="Normal" Path="mjstory/popup/viral_popup_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>