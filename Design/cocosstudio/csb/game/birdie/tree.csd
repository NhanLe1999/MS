<GameFile>
  <PropertyGroup Name="tree" Type="Layer" ID="ae972daf-a19b-49ab-8fb9-8ab96ae7fc5f" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="130" Speed="1.0000" ActivedAnimationName="wrong_answer">
        <Timeline ActionTag="-2088019679" Property="Scale">
          <ScaleFrame FrameIndex="70" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="85" X="0.5000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="100" X="1.0000" Y="0.3000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="115" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="986305845" Property="Scale">
          <ScaleFrame FrameIndex="0" X="1.0000" Y="0.8000">
            <EasingData Type="23" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="62" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="70" X="1.0000" Y="0.8000">
            <EasingData Type="23" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="130" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="right_answer" StartIndex="0" EndIndex="62">
          <RenderColor A="255" R="255" G="105" B="180" />
        </AnimationInfo>
        <AnimationInfo Name="wrong_answer" StartIndex="70" EndIndex="130">
          <RenderColor A="255" R="0" G="0" B="205" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="MJGameBirdieTree" Tag="298" ctype="GameLayerObjectData">
        <Size X="490.0000" Y="555.0000" />
        <Children>
          <AbstractNodeData Name="Panel_7" ActionTag="1670058075" Tag="47" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="490.0000" Y="555.0000" />
            <Children>
              <AbstractNodeData Name="bird" ActionTag="-1295514043" VisibleForFrame="False" Tag="25" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="170.0000" RightMargin="170.0000" TopMargin="-56.0000" BottomMargin="411.0000" LeftEage="49" RightEage="49" TopEage="66" BottomEage="66" Scale9OriginX="49" Scale9OriginY="66" Scale9Width="52" Scale9Height="68" ctype="ImageViewObjectData">
                <Size X="150.0000" Y="200.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="245.0000" Y="511.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.9207" />
                <PreSize X="0.3061" Y="0.3604" />
                <FileData Type="Normal" Path="games/birdie/bird.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="Panel_6" ActionTag="986305845" Tag="42" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="490.0000" Y="555.0000" />
            <Children>
              <AbstractNodeData Name="Image_2" ActionTag="-344069377" Tag="5" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="165.1000" RightMargin="174.9000" TopMargin="480.0000" LeftEage="49" RightEage="49" TopEage="24" BottomEage="24" Scale9OriginX="49" Scale9OriginY="24" Scale9Width="52" Scale9Height="27" ctype="ImageViewObjectData">
                <Size X="150.0000" Y="75.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="240.1000" Y="37.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4900" Y="0.0676" />
                <PreSize X="0.3061" Y="0.1351" />
                <FileData Type="Normal" Path="games/birdie/root.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_2_0" ActionTag="2012140839" Tag="6" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="TopEdge" TopMargin="19.0000" BottomMargin="66.0000" Scale9Enable="True" LeftEage="49" RightEage="49" TopEage="405" BottomEage="50" Scale9OriginX="49" Scale9OriginY="405" Scale9Width="392" Scale9Height="1" ctype="ImageViewObjectData">
                <Size X="490.0000" Y="470.0000" />
                <Children>
                  <AbstractNodeData Name="mouth" ActionTag="-2088019679" Tag="7" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="95.0000" RightMargin="95.0000" TopMargin="382.5000" BottomMargin="12.5000" Scale9Enable="True" LeftEage="96" RightEage="96" TopEage="17" BottomEage="17" Scale9OriginX="96" Scale9OriginY="17" Scale9Width="100" Scale9Height="20" ctype="ImageViewObjectData">
                    <Size X="300.0000" Y="75.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="245.0000" Y="50.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.1064" />
                    <PreSize X="0.6122" Y="0.1596" />
                    <FileData Type="Normal" Path="games/birdie/mouth.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="eye" ActionTag="744224260" Tag="8" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="-5.0000" RightMargin="-5.0000" TopMargin="364.0008" BottomMargin="5.9992" LeftEage="165" RightEage="165" TopEage="33" BottomEage="33" Scale9OriginX="165" Scale9OriginY="33" Scale9Width="170" Scale9Height="34" ctype="ImageViewObjectData">
                    <Size X="500.0000" Y="100.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="245.0000" Y="55.9992" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.1191" />
                    <PreSize X="1.0204" Y="0.2128" />
                    <FileData Type="Normal" Path="games/birdie/eye_close.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="image" ActionTag="882721333" Tag="40" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="65.0000" RightMargin="65.0000" TopMargin="47.3530" BottomMargin="122.6470" Scale9Width="46" Scale9Height="46" ctype="ImageViewObjectData">
                    <Size X="360.0000" Y="300.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="245.0000" Y="272.6470" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5801" />
                    <PreSize X="0.7347" Y="0.6383" />
                    <FileData Type="Default" Path="Default/ImageFile.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="245.0000" Y="301.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5423" />
                <PreSize X="1.0000" Y="0.8468" />
                <FileData Type="Normal" Path="games/birdie/capony.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>