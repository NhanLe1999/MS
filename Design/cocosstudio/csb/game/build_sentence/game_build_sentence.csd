<GameFile>
  <PropertyGroup Name="game_build_sentence" Type="Layer" ID="be78f5a0-e650-4fbc-9ad2-43242c410748" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="210" Speed="1.0000" ActivedAnimationName="appear">
        <Timeline ActionTag="-925579386" Property="Alpha">
          <IntFrame FrameIndex="0" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="60" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="85" Value="0">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
        <Timeline ActionTag="-1922576349" Property="Position">
          <PointFrame FrameIndex="90" X="512.0000" Y="1000.0000">
            <EasingData Type="14" />
          </PointFrame>
          <PointFrame FrameIndex="100" X="512.0000" Y="1000.0000">
            <EasingData Type="14" />
          </PointFrame>
          <PointFrame FrameIndex="150" X="512.0000" Y="575.0000">
            <EasingData Type="13" />
          </PointFrame>
          <PointFrame FrameIndex="160" X="512.0000" Y="575.0000">
            <EasingData Type="13" />
          </PointFrame>
          <PointFrame FrameIndex="210" X="512.0000" Y="1000.0000">
            <EasingData Type="14" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-1922576349" Property="Alpha">
          <IntFrame FrameIndex="90" Value="0">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="100" Value="0">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="115" Value="255">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
        <Timeline ActionTag="1888457631" Property="Position">
          <PointFrame FrameIndex="90" X="512.0000" Y="-500.0000">
            <EasingData Type="14" />
          </PointFrame>
          <PointFrame FrameIndex="150" X="512.0000" Y="115.0000">
            <EasingData Type="13" />
          </PointFrame>
          <PointFrame FrameIndex="160" X="512.0000" Y="115.0000">
            <EasingData Type="13" />
          </PointFrame>
          <PointFrame FrameIndex="210" X="512.0000" Y="-500.0000">
            <EasingData Type="14" />
          </PointFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="appear" StartIndex="90" EndIndex="150">
          <RenderColor A="255" R="255" G="99" B="71" />
        </AnimationInfo>
        <AnimationInfo Name="disappear" StartIndex="160" EndIndex="210">
          <RenderColor A="255" R="255" G="160" B="122" />
        </AnimationInfo>
        <AnimationInfo Name="intro" StartIndex="0" EndIndex="85">
          <RenderColor A="255" R="192" G="192" B="192" />
        </AnimationInfo>
        <AnimationInfo Name="day_to_night" StartIndex="220" EndIndex="260">
          <RenderColor A="255" R="135" G="206" B="235" />
        </AnimationInfo>
        <AnimationInfo Name="night_to_day" StartIndex="265" EndIndex="305">
          <RenderColor A="255" R="205" G="92" B="92" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="MJGameBuildSentence" Tag="42" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="Panel_1" ActionTag="2115903082" Tag="298" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="background_day" ActionTag="-218638577" Tag="43" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-338.0000" RightMargin="-338.0000" LeftEage="462" RightEage="462" TopEage="253" BottomEage="253" Scale9OriginX="462" Scale9OriginY="253" Scale9Width="476" Scale9Height="262" ctype="ImageViewObjectData">
            <Size X="1700.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.6602" Y="1.0000" />
            <FileData Type="Normal" Path="games/build_sentence/background.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="background_night" ActionTag="344892853" Tag="297" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-338.0000" RightMargin="-338.0000" LeftEage="462" RightEage="462" TopEage="253" BottomEage="253" Scale9OriginX="462" Scale9OriginY="253" Scale9Width="476" Scale9Height="262" ctype="ImageViewObjectData">
            <Size X="1700.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0204" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.6602" Y="1.0000" />
            <FileData Type="Normal" Path="games/build_sentence/background_night.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="content" ActionTag="-1157316241" Tag="76" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="Image_2" ActionTag="-925579386" Tag="33" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="113.5000" RightMargin="113.5000" TopMargin="234.0000" BottomMargin="350.0000" LeftEage="263" RightEage="263" TopEage="60" BottomEage="60" Scale9OriginX="263" Scale9OriginY="60" Scale9Width="271" Scale9Height="64" ctype="ImageViewObjectData">
                <Size X="797.0000" Y="184.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="442.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5755" />
                <PreSize X="0.7783" Y="0.2396" />
                <FileData Type="Normal" Path="games/build_sentence/logo.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="frame" ActionTag="-1922576349" Alpha="0" Tag="45" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="262.0000" RightMargin="262.0000" TopMargin="-422.0000" BottomMargin="810.0000" Scale9Enable="True" LeftEage="30" RightEage="30" TopEage="30" BottomEage="30" Scale9OriginX="16" Scale9OriginY="16" Scale9Width="14" Scale9Height="14" ctype="ImageViewObjectData">
                <Size X="500.0000" Y="380.0000" />
                <Children>
                  <AbstractNodeData Name="image" ActionTag="-1216005034" Tag="126" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="20.0000" RightMargin="20.0000" TopMargin="25.0000" BottomMargin="25.0000" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="16" Scale9Height="16" ctype="ImageViewObjectData">
                    <Size X="460.0000" Y="330.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="250.0000" Y="190.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.9200" Y="0.8684" />
                    <FileData Type="Default" Path="Default/ImageFile.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Panel_9" ActionTag="1220897451" Tag="78" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="100.0000" RightMargin="390.0000" TopMargin="-19.0000" BottomMargin="369.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="10.0000" Y="30.0000" />
                    <AnchorPoint />
                    <Position X="100.0000" Y="369.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2000" Y="0.9711" />
                    <PreSize X="0.0200" Y="0.0789" />
                    <SingleColor A="255" R="157" G="102" B="0" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Panel_9_0" ActionTag="572013095" Tag="79" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="TopEdge" LeftMargin="390.0000" RightMargin="100.0000" TopMargin="-19.0000" BottomMargin="369.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="10.0000" Y="30.0000" />
                    <AnchorPoint />
                    <Position X="390.0000" Y="369.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7800" Y="0.9711" />
                    <PreSize X="0.0200" Y="0.0789" />
                    <SingleColor A="255" R="157" G="102" B="0" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="1000.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="1.3021" />
                <PreSize X="0.4883" Y="0.4948" />
                <FileData Type="Default" Path="Default/ImageFile.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="answer_zone" ActionTag="-884306382" Tag="53" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="12.0000" RightMargin="12.0000" TopMargin="168.0000" BottomMargin="200.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="1000.0000" Y="400.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="512.0000" Y="200.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2604" />
                <PreSize X="0.9766" Y="0.5208" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="land" ActionTag="1888457631" Tag="44" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="-188.0000" RightMargin="-188.0000" TopMargin="1129.0000" BottomMargin="-639.0000" LeftEage="462" RightEage="462" TopEage="181" BottomEage="181" Scale9OriginX="462" Scale9OriginY="97" Scale9Width="476" Scale9Height="84" ctype="ImageViewObjectData">
                <Size X="1400.0000" Y="278.0000" />
                <Children>
                  <AbstractNodeData Name="top_stand" ActionTag="-2029936017" Tag="80" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="200.0000" RightMargin="200.0000" TopMargin="-158.0000" BottomMargin="276.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="165" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="1000.0000" Y="160.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="700.0000" Y="356.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="1.2806" />
                    <PreSize X="0.7143" Y="0.5755" />
                    <SingleColor A="255" R="255" G="255" B="0" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="-500.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="-0.6510" />
                <PreSize X="1.3672" Y="0.3620" />
                <FileData Type="Normal" Path="games/build_sentence/land.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
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