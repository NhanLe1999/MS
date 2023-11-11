<GameFile>
  <PropertyGroup Name="ActivityWaiting" Type="Layer" ID="6db4affa-c529-4056-8750-dd08df9fef9a" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="60" Speed="1.0000">
        <Timeline ActionTag="-993283202" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.8000" Y="0.8000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="0.8000" Y="0.8000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-993283202" Property="Alpha">
          <IntFrame FrameIndex="0" Value="152">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="30" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="60" Value="153">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
      </Animation>
      <ObjectData Name="Layer" CustomClassName="MSActivityWait" Tag="66" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="Image_1" ActionTag="-380363588" VisibleForFrame="False" Tag="67" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="BottomEdge" LeftMargin="944.0000" RightMargin="20.0000" TopMargin="688.0000" BottomMargin="20.0000" LeftEage="19" RightEage="19" TopEage="19" BottomEage="19" Scale9OriginX="19" Scale9OriginY="19" Scale9Width="22" Scale9Height="22" ctype="ImageViewObjectData">
            <Size X="60.0000" Y="60.0000" />
            <AnchorPoint ScaleX="0.4417" ScaleY="0.5333" />
            <Position X="970.5020" Y="51.9980" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.9478" Y="0.0677" />
            <PreSize X="0.0586" Y="0.0781" />
            <FileData Type="Normal" Path="mjstory/new_ui/image_monkey_head.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="text_waiting" ActionTag="-993283202" VisibleForFrame="False" Alpha="152" Tag="68" IconVisible="False" HorizontalEdge="RightEdge" LeftMargin="820.6000" RightMargin="109.4000" TopMargin="701.5000" BottomMargin="33.5000" FontSize="25" LabelText="Waiting" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="94.0000" Y="33.0000" />
            <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
            <Position X="914.6000" Y="50.0000" />
            <Scale ScaleX="0.8000" ScaleY="0.8000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.8932" Y="0.0651" />
            <PreSize X="0.0918" Y="0.0430" />
            <FontResource Type="Normal" Path="fonts/IBMPlexSans-Bold.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>