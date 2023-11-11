<GameFile>
  <PropertyGroup Name="StoryTheEndPage" Type="Layer" ID="56d90045-03c1-474e-9efb-54cc5b0cd5c7" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="195" Speed="1.0000" ActivedAnimationName="action">
        <Timeline ActionTag="-2017428530" Property="Scale">
          <ScaleFrame FrameIndex="150" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="175" X="1.3000" Y="1.3000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="185" X="1.3000" Y="1.3000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="195" X="0.0100" Y="0.0100">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="show" StartIndex="0" EndIndex="60">
          <RenderColor A="255" R="221" G="160" B="221" />
        </AnimationInfo>
        <AnimationInfo Name="action" StartIndex="145" EndIndex="235">
          <RenderColor A="255" R="128" G="128" B="128" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="StoryTheEndPage" Tag="121" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="bg" ActionTag="-2017428530" Tag="4" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-188.0000" RightMargin="-188.0000" TopMargin="-6.0000" BottomMargin="-6.0000" TouchEnable="True" LeftEage="212" RightEage="212" TopEage="64" BottomEage="64" Scale9OriginX="212" Scale9OriginY="64" Scale9Width="976" Scale9Height="652" ctype="ImageViewObjectData">
            <Size X="1400.0000" Y="780.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.3672" Y="1.0156" />
            <FileData Type="Normal" Path="mjstory/theendpage/theendpage_bg.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="title" ActionTag="-600001012" Alpha="0" Tag="5" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="328.5000" RightMargin="328.5000" TopMargin="52.4060" BottomMargin="660.5940" FontSize="46" LabelText="You have learned:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="367.0000" Y="55.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
            <Position X="512.0000" Y="715.5940" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="82" G="59" B="58" />
            <PrePosition X="0.5000" Y="0.9318" />
            <PreSize X="0.3584" Y="0.0716" />
            <FontResource Type="Normal" Path="fonts/Linotte Bold.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="layout_content1" ActionTag="1177863443" Tag="73" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TopMargin="1.7037" BottomMargin="-1.7037" TouchEnable="True" ClipAble="False" BackColorAlpha="130" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint />
            <Position Y="-1.7037" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition Y="-0.0022" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="30" G="144" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="btplay" ActionTag="-1960119851" CallBackType="Click" CallBackName="onPlayGame" Tag="13" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="349.0000" RightMargin="349.0000" TopMargin="674.4638" BottomMargin="15.5362" TouchEnable="True" FontSize="40" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="296" Scale9Height="56" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="326.0000" Y="78.0000" />
            <Children>
              <AbstractNodeData Name="btname" ActionTag="-1877761036" Tag="14" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="68.0000" RightMargin="68.0000" TopMargin="15.0000" BottomMargin="15.0000" FontSize="40" LabelText="Play game" OutlineSize="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="190.0000" Y="48.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="163.0000" Y="39.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.5828" Y="0.6154" />
                <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="255" B="255" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btskip" ActionTag="-1478471080" CallBackType="Click" CallBackName="onSkip" Tag="15" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="358.1488" RightMargin="-156.1488" TopMargin="12.0000" BottomMargin="12.0000" TouchEnable="True" FontSize="40" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="94" Scale9Height="32" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="124.0000" Y="54.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="420.1488" Y="39.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="1.2888" Y="0.5000" />
                <PreSize X="0.3804" Y="0.6923" />
                <FontResource Type="Normal" Path="fonts/Linotte Bold.ttf" Plist="" />
                <TextColor A="255" R="140" G="83" B="23" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/theendpage/theendpage_btskip.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="54.5362" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.0710" />
            <PreSize X="0.3184" Y="0.1016" />
            <FontResource Type="Normal" Path="fonts/Linotte Bold.ttf" Plist="" />
            <TextColor A="255" R="140" G="83" B="23" />
            <DisabledFileData Type="Normal" Path="mjstory/theendpage/theendpage_btplay.png" Plist="" />
            <PressedFileData Type="Normal" Path="mjstory/theendpage/theendpage_btplay.png" Plist="" />
            <NormalFileData Type="Normal" Path="mjstory/theendpage/theendpage_btplay.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="layout_content" ActionTag="754292232" Tag="8" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="512.0000" RightMargin="412.0000" TopMargin="668.0000" ClipAble="False" BackColorAlpha="130" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="100.0000" Y="100.0000" />
            <AnchorPoint />
            <Position X="512.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" />
            <PreSize X="0.0977" Y="0.1302" />
            <SingleColor A="255" R="30" G="144" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>