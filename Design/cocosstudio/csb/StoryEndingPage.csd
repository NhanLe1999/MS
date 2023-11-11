<GameFile>
  <PropertyGroup Name="StoryEndingPage" Type="Layer" ID="1b81c9d6-dd00-4bb8-bff8-f68fd13dc397" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="185" Speed="1.0000" ActivedAnimationName="action">
        <Timeline ActionTag="1177863443" Property="Alpha">
          <IntFrame FrameIndex="0" Value="0">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="40" Value="216">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
        <Timeline ActionTag="286062415" Property="Position">
          <PointFrame FrameIndex="150" X="532.9500" Y="-164.6000">
            <EasingData Type="26" />
          </PointFrame>
          <PointFrame FrameIndex="185" X="516.8000" Y="320.0000">
            <EasingData Type="26" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="286062415" Property="Scale">
          <ScaleFrame FrameIndex="150" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-1659749545" Property="Position">
          <PointFrame FrameIndex="150" X="807.5000" Y="-164.6000">
            <EasingData Type="26" />
          </PointFrame>
          <PointFrame FrameIndex="185" X="807.5000" Y="320.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="567844123" Property="Position">
          <PointFrame FrameIndex="150" X="1098.2000" Y="-164.6000">
            <EasingData Type="26" />
          </PointFrame>
          <PointFrame FrameIndex="185" X="1098.2000" Y="320.0000">
            <EasingData Type="26" />
          </PointFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="show" StartIndex="0" EndIndex="60">
          <RenderColor A="255" R="221" G="160" B="221" />
        </AnimationInfo>
        <AnimationInfo Name="action" StartIndex="145" EndIndex="190">
          <RenderColor A="255" R="128" G="128" B="128" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="StoryEndingPage" Tag="121" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="root_layout" ActionTag="1177863443" Alpha="0" Tag="73" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="26" G="26" B="26" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="bg" ActionTag="-998565279" Tag="68" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-295.5000" RightMargin="-295.5000" TopMargin="-6.0000" BottomMargin="-6.0000" LeftEage="493" RightEage="493" TopEage="271" BottomEage="271" Scale9OriginX="493" Scale9OriginY="271" Scale9Width="629" Scale9Height="238" ctype="ImageViewObjectData">
            <Size X="1615.0000" Y="780.0000" />
            <Children>
              <AbstractNodeData Name="queslb" ActionTag="838354335" Alpha="0" Tag="13" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="560.5000" RightMargin="560.5000" TopMargin="143.4720" BottomMargin="576.5280" FontSize="50" LabelText="Do you like this story?" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="494.0000" Y="60.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="807.5000" Y="606.5280" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="82" G="59" B="58" />
                <PrePosition X="0.5000" Y="0.7776" />
                <PreSize X="0.3059" Y="0.0769" />
                <FontResource Type="Normal" Path="fonts/Linotte Bold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="suggestlb" ActionTag="2055265247" Tag="14" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="379.8480" RightMargin="883.1520" TopMargin="375.1560" BottomMargin="374.8440" FontSize="25" LabelText="Our recommended stories:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="352.0000" Y="30.0000" />
                <AnchorPoint ScaleY="0.5000" />
                <Position X="379.8480" Y="389.8440" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.2352" Y="0.4998" />
                <PreSize X="0.2180" Y="0.0385" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="face1" ActionTag="286062415" CallBackType="Click" CallBackName="onRate" Tag="11" IconVisible="False" LeftMargin="432.9500" RightMargin="982.0500" TopMargin="844.6000" BottomMargin="-264.6000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="200.0000" Y="200.0000" />
                <Children>
                  <AbstractNodeData Name="text" ActionTag="-1174946257" Alpha="0" Tag="10" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-7.5000" RightMargin="-7.5000" TopMargin="223.5000" BottomMargin="-96.5000" FontSize="60" LabelText="I love it" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="215.0000" Y="73.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="100.0000" Y="-60.0000" />
                    <Scale ScaleX="0.8000" ScaleY="0.8000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5000" Y="-0.3000" />
                    <PreSize X="1.0750" Y="0.3650" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="532.9500" Y="-164.6000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3300" Y="-0.2110" />
                <PreSize X="0.1238" Y="0.2564" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="face2" ActionTag="-1659749545" CallBackType="Click" CallBackName="onRate" Tag="12" IconVisible="False" LeftMargin="707.5000" RightMargin="707.5000" TopMargin="844.6000" BottomMargin="-264.6000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="200.0000" Y="200.0000" />
                <Children>
                  <AbstractNodeData Name="text" ActionTag="2042283664" Alpha="0" Tag="11" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="12.0000" RightMargin="12.0000" TopMargin="223.5000" BottomMargin="-96.5000" FontSize="60" LabelText="It's ok" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="176.0000" Y="73.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="100.0000" Y="-60.0000" />
                    <Scale ScaleX="0.8000" ScaleY="0.8000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5000" Y="-0.3000" />
                    <PreSize X="0.8800" Y="0.3650" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="807.5000" Y="-164.6000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="-0.2110" />
                <PreSize X="0.1238" Y="0.2564" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="face3" ActionTag="567844123" CallBackType="Click" CallBackName="onRate" Tag="13" IconVisible="False" LeftMargin="998.2000" RightMargin="416.8000" TopMargin="844.6000" BottomMargin="-264.6000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="200.0000" Y="200.0000" />
                <Children>
                  <AbstractNodeData Name="text" ActionTag="645402170" Alpha="0" Tag="12" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-54.0000" RightMargin="-54.0000" TopMargin="223.5000" BottomMargin="-96.5000" FontSize="60" LabelText="No, I don't" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="308.0000" Y="73.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="100.0000" Y="-60.0000" />
                    <Scale ScaleX="0.8000" ScaleY="0.8000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5000" Y="-0.3000" />
                    <PreSize X="1.5400" Y="0.3650" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="1098.2000" Y="-164.6000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.6800" Y="-0.2110" />
                <PreSize X="0.1238" Y="0.2564" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="list_suggest" ActionTag="-1301536293" VisibleForFrame="False" Tag="153" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="387.5000" RightMargin="387.5000" TopMargin="430.0000" BottomMargin="100.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="0" ItemMargin="20" VerticalType="Align_Bottom" ctype="ListViewObjectData">
                <Size X="840.0000" Y="250.0000" />
                <Children>
                  <AbstractNodeData Name="Panel_3" ActionTag="930985387" Tag="167" IconVisible="False" RightMargin="570.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="270.0000" Y="250.0000" />
                    <Children>
                      <AbstractNodeData Name="Panel_4" ActionTag="752848240" Tag="168" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-0.5000" RightMargin="269.5000" TopMargin="25.0000" BottomMargin="25.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="127" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="1.0000" Y="200.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position Y="125.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition Y="0.5000" />
                        <PreSize X="0.0037" Y="0.8000" />
                        <SingleColor A="255" R="0" G="0" B="0" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="button_recommend" ActionTag="-1195818135" CallBackType="Click" CallBackName="onRecommendTab" Tag="106" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="RightEdge" LeftMargin="20.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="6" RightEage="6" TopEage="11" BottomEage="11" Scale9OriginX="6" Scale9OriginY="11" Scale9Width="8" Scale9Height="1" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="250.0000" Y="250.0000" />
                        <Children>
                          <AbstractNodeData Name="label_recommend" ActionTag="978835940" Tag="107" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="43.0000" RightMargin="43.0000" TopMargin="188.0000" BottomMargin="25.0000" FontSize="30" LabelText="Text Label" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="164.0000" Y="37.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="125.0000" Y="43.5000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.1740" />
                            <PreSize X="0.6560" Y="0.1480" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="145.0000" Y="125.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="38" G="176" B="230" />
                        <PrePosition X="0.5370" Y="0.5000" />
                        <PreSize X="0.9259" Y="1.0000" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                        <PressedFileData Type="Normal" Path="mjstory/new_ui/frame_gradebox_background.png" Plist="" />
                        <NormalFileData Type="Normal" Path="mjstory/new_ui/frame_gradebox_background.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="0.3214" Y="1.0000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="807.5000" Y="225.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2885" />
                <PreSize X="0.5201" Y="0.3205" />
                <SingleColor A="255" R="150" G="150" B="255" />
                <FirstColor A="255" R="150" G="150" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.5771" Y="1.0156" />
            <FileData Type="Normal" Path="mjstory/theendpage/theendpage_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>