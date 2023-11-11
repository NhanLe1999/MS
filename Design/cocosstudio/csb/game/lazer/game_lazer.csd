<GameFile>
  <PropertyGroup Name="game_lazer" Type="Layer" ID="858d9c6b-d6a9-42d9-8277-e269182fb3d9" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="255" Speed="1.0000">
        <Timeline ActionTag="-1384188803" Property="Position">
          <PointFrame FrameIndex="0" X="512.0000" Y="1000.0000">
            <EasingData Type="14" />
          </PointFrame>
          <PointFrame FrameIndex="60" X="512.0000" Y="598.0000">
            <EasingData Type="13" />
          </PointFrame>
          <PointFrame FrameIndex="120" X="512.0000" Y="1400.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-1346849374" Property="Position">
          <PointFrame FrameIndex="0" X="512.0000" Y="-402.0000">
            <EasingData Type="14" />
          </PointFrame>
          <PointFrame FrameIndex="60" X="512.0000" Y="0.0000">
            <EasingData Type="13" />
          </PointFrame>
          <PointFrame FrameIndex="120" X="512.0000" Y="-402.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-2112067918" Property="Position">
          <PointFrame FrameIndex="155" X="512.0000" Y="384.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-2112067918" Property="RotationSkew">
          <ScaleFrame FrameIndex="215" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-2112067918" Property="Alpha">
          <IntFrame FrameIndex="215" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="255" Value="0">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
        <Timeline ActionTag="-2112067918" Property="VisibleForFrame">
          <BoolFrame FrameIndex="0" Tween="False" Value="False" />
          <BoolFrame FrameIndex="60" Tween="False" Value="False" />
          <BoolFrame FrameIndex="149" Tween="False" Value="False" />
          <BoolFrame FrameIndex="150" Tween="False" Value="True" />
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="appear" StartIndex="0" EndIndex="60">
          <RenderColor A="255" R="255" G="69" B="0" />
        </AnimationInfo>
        <AnimationInfo Name="disappear" StartIndex="60" EndIndex="149">
          <RenderColor A="255" R="106" G="90" B="205" />
        </AnimationInfo>
        <AnimationInfo Name="logo" StartIndex="150" EndIndex="255">
          <RenderColor A="255" R="255" G="228" B="225" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="MJGameLazer" Tag="20" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="background" ActionTag="1335363537" Tag="21" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" ComboBoxIndex="2" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="Panel_1" ActionTag="841887789" Tag="70" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="1024.0000" Y="768.0000" />
                <Children>
                  <AbstractNodeData Name="top_frames" ActionTag="-1384188803" Tag="29" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="TopEdge" TopMargin="-402.0000" BottomMargin="830.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="1024.0000" Y="340.0000" />
                    <Children>
                      <AbstractNodeData Name="FileNode_1" ActionTag="-1482234054" Tag="30" IconVisible="True" HorizontalEdge="LeftEdge" LeftMargin="10.0000" RightMargin="784.0000" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
                        <Size X="230.0000" Y="340.0000" />
                        <AnchorPoint />
                        <Position X="10.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.0098" />
                        <PreSize X="0.2246" Y="1.0000" />
                        <FileData Type="Normal" Path="csb/game/lazer/start_point.csd" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="FileNode_1_0" ActionTag="-19487136" Tag="35" IconVisible="True" HorizontalEdge="LeftEdge" LeftMargin="268.0000" RightMargin="526.0000" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
                        <Size X="230.0000" Y="340.0000" />
                        <AnchorPoint />
                        <Position X="268.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.2617" />
                        <PreSize X="0.2246" Y="1.0000" />
                        <FileData Type="Normal" Path="csb/game/lazer/start_point.csd" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="FileNode_1_1" ActionTag="-306193998" Tag="40" IconVisible="True" LeftMargin="526.0000" RightMargin="268.0000" TopMargin="0.0001" BottomMargin="-0.0001" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
                        <Size X="230.0000" Y="340.0000" />
                        <AnchorPoint />
                        <Position X="526.0000" Y="-0.0001" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5137" Y="0.0000" />
                        <PreSize X="0.2246" Y="1.0000" />
                        <FileData Type="Normal" Path="csb/game/lazer/start_point.csd" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="FileNode_1_2" ActionTag="-959072040" Tag="45" IconVisible="True" HorizontalEdge="RightEdge" LeftMargin="784.0000" RightMargin="10.0000" TopMargin="0.0001" BottomMargin="-0.0001" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
                        <Size X="230.0000" Y="340.0000" />
                        <AnchorPoint />
                        <Position X="784.0000" Y="-0.0001" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.7656" Y="0.0000" />
                        <PreSize X="0.2246" Y="1.0000" />
                        <FileData Type="Normal" Path="csb/game/lazer/start_point.csd" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="512.0000" Y="1000.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="1.3021" />
                    <PreSize X="1.0000" Y="0.4427" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="bottom_frames" ActionTag="-1346849374" Tag="50" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" TopMargin="840.0000" BottomMargin="-402.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="1024.0000" Y="330.0000" />
                    <Children>
                      <AbstractNodeData Name="FileNode_2" ActionTag="65432709" Tag="71" IconVisible="True" HorizontalEdge="LeftEdge" VerticalEdge="BottomEdge" LeftMargin="10.0000" RightMargin="784.0000" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
                        <Size X="230.0000" Y="330.0000" />
                        <AnchorPoint />
                        <Position X="10.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.0098" />
                        <PreSize X="0.2246" Y="1.0000" />
                        <FileData Type="Normal" Path="csb/game/lazer/end_point.csd" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="FileNode_2_0" ActionTag="739937113" Tag="75" IconVisible="True" HorizontalEdge="RightEdge" VerticalEdge="BottomEdge" LeftMargin="268.0000" RightMargin="526.0000" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
                        <Size X="230.0000" Y="330.0000" />
                        <AnchorPoint />
                        <Position X="268.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.2617" />
                        <PreSize X="0.2246" Y="1.0000" />
                        <FileData Type="Normal" Path="csb/game/lazer/end_point.csd" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="FileNode_2_1" ActionTag="2019202479" Tag="79" IconVisible="True" HorizontalEdge="RightEdge" VerticalEdge="BottomEdge" LeftMargin="526.0000" RightMargin="268.0000" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
                        <Size X="230.0000" Y="330.0000" />
                        <AnchorPoint />
                        <Position X="526.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5137" />
                        <PreSize X="0.2246" Y="1.0000" />
                        <FileData Type="Normal" Path="csb/game/lazer/end_point.csd" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="FileNode_2_2" ActionTag="88160860" Tag="83" IconVisible="True" HorizontalEdge="RightEdge" VerticalEdge="BottomEdge" LeftMargin="784.0000" RightMargin="10.0000" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
                        <Size X="230.0000" Y="330.0000" />
                        <AnchorPoint />
                        <Position X="784.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.7656" />
                        <PreSize X="0.2246" Y="1.0000" />
                        <FileData Type="Normal" Path="csb/game/lazer/end_point.csd" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" />
                    <Position X="512.0000" Y="-402.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="-0.5234" />
                    <PreSize X="1.0000" Y="0.4297" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_1" ActionTag="-2112067918" VisibleForFrame="False" Tag="169" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="112.0000" RightMargin="112.0000" TopMargin="296.5000" BottomMargin="296.5000" LeftEage="264" RightEage="264" TopEage="57" BottomEage="57" Scale9OriginX="264" Scale9OriginY="57" Scale9Width="272" Scale9Height="61" ctype="ImageViewObjectData">
                    <Size X="800.0000" Y="175.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="512.0000" Y="384.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.7813" Y="0.2279" />
                    <FileData Type="Normal" Path="games/lazer/logo.png" Plist="" />
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
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="255" G="250" B="229" />
            <EndColor A="255" R="69" G="241" B="249" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>