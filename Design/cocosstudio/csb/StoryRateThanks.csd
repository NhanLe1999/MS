<GameFile>
  <PropertyGroup Name="StoryRateThanks" Type="Layer" ID="8730d6e0-8dc4-4a50-8eb2-6395d1d2db70" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="90" Speed="1.0000">
        <Timeline ActionTag="974546587" Property="Alpha">
          <IntFrame FrameIndex="0" Value="0">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="30" Value="216">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="60" Value="217">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="90" Value="0">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
        <Timeline ActionTag="1994306121" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.0100" Y="0.0100">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="1.0000" Y="1.0000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="1.0000" Y="1.0000">
            <EasingData Type="25" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="90" X="0.0100" Y="0.0100">
            <EasingData Type="25" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="show" StartIndex="0" EndIndex="35">
          <RenderColor A="255" R="255" G="240" B="245" />
        </AnimationInfo>
        <AnimationInfo Name="hide" StartIndex="60" EndIndex="95">
          <RenderColor A="255" R="106" G="90" B="205" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="layer" CustomClassName="StoryRatePopUp" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="layout" ActionTag="974546587" Alpha="216" Tag="58" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FlipY="True" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="281" Scale9Height="66" ctype="ImageViewObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="-1.0000" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="mjstory/explore/mjsh_explore_bar.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="rate_bg" ActionTag="1994306121" Tag="56" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="199.0000" RightMargin="199.0000" TopMargin="28.5996" BottomMargin="57.4004" LeftEage="152" RightEage="152" TopEage="171" BottomEage="171" Scale9OriginX="152" Scale9OriginY="171" Scale9Width="322" Scale9Height="340" ctype="ImageViewObjectData">
            <Size X="626.0000" Y="682.0000" />
            <Children>
              <AbstractNodeData Name="title" ActionTag="1282870755" Tag="57" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="130.5000" RightMargin="130.5000" TopMargin="378.4978" BottomMargin="242.5022" FontSize="50" LabelText="CẢM ƠN BẠN!" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="365.0000" Y="61.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="313.0000" Y="273.0022" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="41" G="168" B="211" />
                <PrePosition X="0.5000" Y="0.4003" />
                <PreSize X="0.5831" Y="0.0894" />
                <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btclose" ActionTag="514207473" CallBackType="Click" CallBackName="onClose" Tag="60" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="49.4976" RightMargin="52.5024" TopMargin="544.3484" BottomMargin="47.6516" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="494" Scale9Height="68" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="524.0000" Y="90.0000" />
                <Children>
                  <AbstractNodeData Name="close_title" ActionTag="-1752804287" Tag="61" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="206.0000" RightMargin="206.0000" TopMargin="25.5000" BottomMargin="25.5000" FontSize="32" LabelText="CLOSE" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="112.0000" Y="39.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="262.0000" Y="45.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.2137" Y="0.4333" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="311.4976" Y="92.6516" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4976" Y="0.1359" />
                <PreSize X="0.8371" Y="0.1320" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="mjstory/popup/popup_btok.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/popup/popup_btok.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="398.4004" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5188" />
            <PreSize X="0.6113" Y="0.8880" />
            <FileData Type="Normal" Path="mjstory/popup/popup_ratebuy_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>