<GameFile>
  <PropertyGroup Name="StoryInfoCover_Face" Type="Layer" ID="206e22fb-7ddb-4500-8eb0-bad37f0def9c" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="65" Speed="1.0000" ActivedAnimationName="show">
        <Timeline ActionTag="-290557222" Property="Alpha">
          <IntFrame FrameIndex="50" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="65" Value="0">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
        <Timeline ActionTag="-1503980240" Property="Scale">
          <ScaleFrame FrameIndex="50" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="65" X="0.0100" Y="0.0100">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-1503980240" Property="RotationSkew">
          <ScaleFrame FrameIndex="50" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="65" X="360.0000" Y="360.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-806322282" Property="Position">
          <PointFrame FrameIndex="50" X="512.0000" Y="384.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="65" X="512.0000" Y="384.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-806322282" Property="Scale">
          <ScaleFrame FrameIndex="50" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="65" X="0.0010" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-806322282" Property="RotationSkew">
          <ScaleFrame FrameIndex="50" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="65" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="show" StartIndex="0" EndIndex="35">
          <RenderColor A="255" R="221" G="160" B="221" />
        </AnimationInfo>
        <AnimationInfo Name="hide" StartIndex="50" EndIndex="66">
          <RenderColor A="255" R="255" G="99" B="71" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="StoryInfoCover" Tag="121" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="panel_bg" ActionTag="-290557222" CallBackName="onClose" Tag="46" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="204" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
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
          <AbstractNodeData Name="img_book" ActionTag="-1127956774" Tag="23" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="62.0000" RightMargin="62.0000" TopMargin="34.0000" BottomMargin="34.0000" Scale9Enable="True" LeftEage="13" RightEage="13" TopEage="13" BottomEage="13" Scale9OriginX="13" Scale9OriginY="13" Scale9Width="226" Scale9Height="233" ctype="ImageViewObjectData">
            <Size X="900.0000" Y="700.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.8789" Y="0.9115" />
            <FileData Type="Normal" Path="mjstory/new_ui/tabsubscribe_planbg.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="content" ActionTag="-806322282" Tag="212" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="62.0000" RightMargin="62.0000" TopMargin="34.0000" BottomMargin="34.0000" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="900.0000" Y="700.0000" />
            <Children>
              <AbstractNodeData Name="listview" ActionTag="-355849217" Tag="39" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="TopEdge" LeftMargin="50.8100" RightMargin="49.1901" TopMargin="30.0000" BottomMargin="320.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="0" DirectionType="Vertical" HorizontalType="Align_HorizontalCenter" ctype="ListViewObjectData">
                <Size X="800.0000" Y="350.0000" />
                <Children>
                  <AbstractNodeData Name="story_title" ActionTag="-503919610" Tag="37" IconVisible="False" LeftMargin="114.5000" RightMargin="114.5000" BottomMargin="330.0000" FontSize="35" LabelText="Shapes and colors of the Moon" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="571.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="400.0000" Y="351.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5000" Y="0.9424" />
                    <PreSize X="0.7138" Y="0.1153" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-ExtraBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Panel_5" ActionTag="1347206613" ZOrder="1" Tag="126" IconVisible="False" LeftMargin="300.0000" RightMargin="300.0000" TopMargin="43.0000" BottomMargin="290.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="200.0000" Y="40.0000" />
                    <Children>
                      <AbstractNodeData Name="story_subtitle" ActionTag="-772582465" ZOrder="2" Tag="38" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-400.0000" RightMargin="-400.0000" BottomMargin="-10.0000" IsCustomSize="True" FontSize="25" LabelText="Mia's first experiences series" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="1000.0000" Y="50.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                        <Position X="100.0000" Y="40.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="127" G="127" B="127" />
                        <PrePosition X="0.5000" Y="1.0000" />
                        <PreSize X="5.0000" Y="1.2500" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-MediumItalic.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="300.0000" Y="290.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3750" Y="0.7775" />
                    <PreSize X="0.2500" Y="0.1072" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="panel_detail" ActionTag="-92167417" ZOrder="2" Tag="44" IconVisible="False" TopMargin="83.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="800.0000" Y="290.0000" />
                    <Children>
                      <AbstractNodeData Name="thumb" ActionTag="-1975482474" Tag="77" IconVisible="False" RightMargin="587.5000" TopMargin="-35.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="212.5000" Y="325.0000" />
                        <AnchorPoint />
                        <Position />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition />
                        <PreSize X="0.2656" Y="1.1207" />
                        <SingleColor A="255" R="150" G="200" B="255" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="story_written" ActionTag="2499102" ZOrder="3" Tag="40" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="250.0000" RightMargin="449.0000" TopMargin="116.5000" BottomMargin="151.5000" FontSize="18" LabelText="Written By" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="101.0000" Y="22.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="250.0000" Y="162.5000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="127" G="127" B="127" />
                        <PrePosition X="0.3125" Y="0.5603" />
                        <PreSize X="0.1262" Y="0.0759" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="story_illustrated" ActionTag="841794899" ZOrder="3" Tag="41" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="BottomEdge" LeftMargin="250.0000" RightMargin="429.0000" TopMargin="215.0000" BottomMargin="53.0000" FontSize="18" LabelText="Illustrated by" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="121.0000" Y="22.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="250.0000" Y="64.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="127" G="127" B="127" />
                        <PrePosition X="0.3125" Y="0.2207" />
                        <PreSize X="0.1513" Y="0.0759" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="story_designer" ActionTag="1281337571" ZOrder="3" Tag="42" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="BottomEdge" LeftMargin="250.0000" RightMargin="343.0000" TopMargin="240.0000" BottomMargin="20.0000" FontSize="25" LabelText="Luke Skywalker" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="207.0000" Y="30.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="250.0000" Y="35.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.3125" Y="0.1207" />
                        <PreSize X="0.2587" Y="0.1034" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="story_author" ActionTag="582618831" ZOrder="3" Tag="43" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="250.0000" RightMargin="433.0000" TopMargin="143.5000" BottomMargin="116.5000" FontSize="25" LabelText="Han Solo" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="117.0000" Y="30.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="250.0000" Y="131.5000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.3125" Y="0.4534" />
                        <PreSize X="0.1462" Y="0.1034" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="story_difficulty" ActionTag="-997414083" ZOrder="3" Tag="113" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="250.0000" RightMargin="466.0000" TopMargin="20.0000" BottomMargin="248.0000" FontSize="18" LabelText="Difficulty" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="84.0000" Y="22.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="250.0000" Y="259.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="127" G="127" B="127" />
                        <PrePosition X="0.3125" Y="0.8931" />
                        <PreSize X="0.1050" Y="0.0759" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="story_level" ActionTag="-766331117" ZOrder="3" Tag="114" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="250.0000" RightMargin="528.0000" TopMargin="47.0000" BottomMargin="213.0000" FontSize="25" LabelText="A" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="22.0000" Y="30.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="250.0000" Y="228.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.3125" Y="0.7862" />
                        <PreSize X="0.0275" Y="0.1034" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="panel_face" ActionTag="-1229835402" Tag="190" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="RightEdge" LeftMargin="600.0000" TopMargin="45.0000" BottomMargin="45.0000" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="200.0000" Y="200.0000" />
                        <Children>
                          <AbstractNodeData Name="face_mask" ActionTag="-108738163" Alpha="102" CallBackType="Click" CallBackName="onAddFace" Tag="194" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="25.0000" RightMargin="25.0000" TopMargin="75.0000" BottomMargin="-25.0000" TouchEnable="True" LeftEage="33" RightEage="33" TopEage="33" BottomEage="33" Scale9OriginX="33" Scale9OriginY="33" Scale9Width="34" Scale9Height="34" ctype="ImageViewObjectData">
                            <Size X="150.0000" Y="150.0000" />
                            <AnchorPoint />
                            <Position X="25.0000" Y="-25.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.1250" Y="-0.1250" />
                            <PreSize X="0.7500" Y="0.7500" />
                            <FileData Type="Normal" Path="mjstory/new_ui/face_placeholder.png" Plist="" />
                          </AbstractNodeData>
                          <AbstractNodeData Name="Image_1" ActionTag="2027734261" Tag="191" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-50.0000" RightMargin="-50.0000" TopMargin="-30.0000" BottomMargin="130.0000" Scale9Enable="True" LeftEage="26" RightEage="26" TopEage="26" BottomEage="26" Scale9OriginX="26" Scale9OriginY="26" Scale9Width="28" Scale9Height="28" ctype="ImageViewObjectData">
                            <Size X="300.0000" Y="100.0000" />
                            <Children>
                              <AbstractNodeData Name="Image_2" ActionTag="-1538002672" Tag="192" IconVisible="False" PositionPercentXEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="110.0000" RightMargin="110.0000" Scale9Enable="True" LeftEage="26" RightEage="26" TopEage="26" BottomEage="26" Scale9OriginX="26" Scale9OriginY="26" Scale9Width="28" Scale9Height="28" ctype="ImageViewObjectData">
                                <Size X="80.0000" Y="100.0000" />
                                <AnchorPoint ScaleX="0.5000" />
                                <Position X="150.0000" />
                                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                                <CColor A="255" R="255" G="255" B="255" />
                                <PrePosition X="0.5000" />
                                <PreSize X="0.2667" Y="1.0000" />
                                <FileData Type="Normal" Path="mjstory/new_ui/inline_text_background_arrow.png" Plist="" />
                              </AbstractNodeData>
                              <AbstractNodeData Name="Text_1" ActionTag="-662466990" Alpha="127" Tag="193" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="53.0000" RightMargin="53.0000" TopMargin="36.5000" BottomMargin="36.5000" FontSize="22" LabelText="Tap to add a face" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                                <Size X="194.0000" Y="27.0000" />
                                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                                <Position X="150.0000" Y="50.0000" />
                                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                                <CColor A="255" R="0" G="0" B="0" />
                                <PrePosition X="0.5000" Y="0.5000" />
                                <PreSize X="0.6467" Y="0.2700" />
                                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                                <OutlineColor A="255" R="255" G="0" B="0" />
                                <ShadowColor A="255" R="110" G="110" B="110" />
                              </AbstractNodeData>
                            </Children>
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="100.0000" Y="180.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.9000" />
                            <PreSize X="1.5000" Y="0.5000" />
                            <FileData Type="Normal" Path="mjstory/new_ui/inline_text_background.png" Plist="" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="700.0000" Y="145.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.8750" Y="0.5000" />
                        <PreSize X="0.2500" Y="0.6897" />
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
                    <PreSize X="1.0000" Y="0.7775" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="450.8100" Y="495.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5009" Y="0.7071" />
                <PreSize X="0.8889" Y="0.5000" />
                <SingleColor A="255" R="150" G="150" B="255" />
                <FirstColor A="255" R="150" G="150" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="panel_download" ActionTag="-597444152" VisibleForFrame="False" Tag="32" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="250.0000" RightMargin="250.0000" TopMargin="415.0000" BottomMargin="-15.0000" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="400.0000" Y="300.0000" />
                <Children>
                  <AbstractNodeData Name="btdownload" ActionTag="1897983621" CallBackType="Click" CallBackName="onDownload" Tag="33" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="69.5000" RightMargin="69.5000" TopMargin="19.5000" BottomMargin="19.5000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="231" Scale9Height="239" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="261.0000" Y="261.0000" />
                    <Children>
                      <AbstractNodeData Name="text_btdownload" ActionTag="-2057047211" Tag="38" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="59.5000" RightMargin="59.5000" TopMargin="142.1000" BottomMargin="89.9000" FontSize="22" LabelText="Read it now" HorizontalAlignmentType="HT_Center" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="142.0000" Y="29.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="130.5000" Y="104.4000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.4000" />
                        <PreSize X="0.5441" Y="0.1111" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="60" G="135" B="37" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="text_filesize" ActionTag="1455162153" Tag="42" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="95.0000" RightMargin="95.0000" TopMargin="173.3100" BottomMargin="63.6900" FontSize="18" LabelText="2.33Mb" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="71.0000" Y="24.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="130.5000" Y="75.6900" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.2900" />
                        <PreSize X="0.2720" Y="0.0920" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="60" G="135" B="37" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.0000" Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.6525" Y="0.8700" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Normal" Path="mjstory/new_ui/button_cover_download_pressed.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/button_cover_download.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_downloadguide" ActionTag="10337274" VisibleForFrame="False" Tag="37" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="109.0000" RightMargin="109.0000" TopMargin="136.0800" BottomMargin="136.9200" FontSize="22" LabelText="Enjoy the story!" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="182.0000" Y="27.0000" />
                    <AnchorPoint ScaleX="0.5000" />
                    <Position X="200.0000" Y="136.9200" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="60" G="77" B="153" />
                    <PrePosition X="0.5000" Y="0.4564" />
                    <PreSize X="0.4550" Y="0.0900" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="450.0000" Y="135.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1929" />
                <PreSize X="0.4444" Y="0.4286" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="panel_progress" ActionTag="332005339" VisibleForFrame="False" Tag="39" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="250.0000" RightMargin="250.0000" TopMargin="415.0000" BottomMargin="-15.0000" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="400.0000" Y="300.0000" />
                <Children>
                  <AbstractNodeData Name="Image_3" ActionTag="-616667583" Tag="84" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="69.5000" RightMargin="69.5000" TopMargin="19.5000" BottomMargin="19.5000" LeftEage="86" RightEage="86" TopEage="86" BottomEage="86" Scale9OriginX="86" Scale9OriginY="86" Scale9Width="89" Scale9Height="89" ctype="ImageViewObjectData">
                    <Size X="261.0000" Y="261.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.0000" Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.6525" Y="0.8700" />
                    <FileData Type="Normal" Path="mjstory/new_ui/button_cover_progress_background.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="loading_bar" ActionTag="506055202" Tag="83" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="106.0000" RightMargin="106.0000" TopMargin="56.0000" BottomMargin="56.0000" ProgressInfo="49" ctype="LoadingBarObjectData">
                    <Size X="188.0000" Y="188.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.0000" Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.4700" Y="0.6267" />
                    <ImageFileData Type="Normal" Path="mjstory/new_ui/button_cover_progress.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="progressbg" ActionTag="-377994518" VisibleForFrame="False" Tag="43" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="1.5000" RightMargin="1.5000" TopMargin="177.0000" BottomMargin="39.0000" LeftEage="103" RightEage="103" TopEage="21" BottomEage="21" Scale9OriginX="103" Scale9OriginY="21" Scale9Width="191" Scale9Height="42" ctype="ImageViewObjectData">
                    <Size X="397.0000" Y="84.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.0000" Y="81.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.2700" />
                    <PreSize X="0.9925" Y="0.2800" />
                    <FileData Type="Normal" Path="mjstory/storyinfo_progressbg.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_status" ActionTag="-2016686706" VisibleForFrame="False" Tag="41" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="104.5000" RightMargin="104.5000" TopMargin="135.0800" BottomMargin="127.9200" FontSize="30" LabelText="Please wait..." HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="191.0000" Y="37.0000" />
                    <AnchorPoint ScaleX="0.5000" />
                    <Position X="200.0000" Y="127.9200" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="108" G="146" B="244" />
                    <PrePosition X="0.5000" Y="0.4264" />
                    <PreSize X="0.4775" Y="0.1233" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_percent" ActionTag="998357155" Tag="42" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="163.5000" RightMargin="163.5000" TopMargin="129.5000" BottomMargin="129.5000" FontSize="30" LabelText="70%" HorizontalAlignmentType="HT_Center" OutlineSize="2" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="73.0000" Y="41.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.0000" Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.1825" Y="0.1367" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Black.ttf" Plist="" />
                    <OutlineColor A="255" R="60" G="135" B="37" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="450.0000" Y="135.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1929" />
                <PreSize X="0.4444" Y="0.4286" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="panel_read" ActionTag="2081425533" VisibleForFrame="False" Tag="29" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="250.0000" RightMargin="250.0000" TopMargin="415.0000" BottomMargin="-15.0000" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="400.0000" Y="300.0000" />
                <Children>
                  <AbstractNodeData Name="btreadtome" ActionTag="1608171577" CallBackType="Click" CallBackName="onPlayStory" Tag="31" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="69.5000" RightMargin="69.5000" TopMargin="19.5000" BottomMargin="19.5000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="231" Scale9Height="239" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="261.0000" Y="261.0000" />
                    <Children>
                      <AbstractNodeData Name="text_readtome" ActionTag="369153193" Tag="48" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="63.0000" RightMargin="63.0000" TopMargin="163.9800" BottomMargin="70.0200" FontSize="22" LabelText="Read to me" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="135.0000" Y="27.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="130.5000" Y="83.5200" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.3200" />
                        <PreSize X="0.5172" Y="0.1034" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.0000" Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.6525" Y="0.8700" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/button_cover_read.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btautoplay" ActionTag="-58167233" CallBackType="Click" CallBackName="onPlayStory" Tag="32" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-90.0000" RightMargin="310.0000" TopMargin="60.0000" BottomMargin="60.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="231" Scale9Height="239" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="180.0000" Y="180.0000" />
                    <Children>
                      <AbstractNodeData Name="text_autoplay" ActionTag="901000450" Tag="49" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="50.5000" RightMargin="50.5000" TopMargin="112.4000" BottomMargin="47.6000" FontSize="16" LabelText="Autoplay" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="79.0000" Y="20.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="90.0000" Y="57.6000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.3200" />
                        <PreSize X="0.4389" Y="0.1111" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition Y="0.5000" />
                    <PreSize X="0.4500" Y="0.6000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/button_cover_autoplay.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btreaditmyself" ActionTag="-705836569" CallBackType="Click" CallBackName="onPlayStory" Tag="33" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="310.0000" RightMargin="-90.0000" TopMargin="60.0000" BottomMargin="60.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="231" Scale9Height="239" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="180.0000" Y="180.0000" />
                    <Children>
                      <AbstractNodeData Name="text_readitmyseft" ActionTag="454238528" Tag="50" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="58.5000" RightMargin="58.5000" TopMargin="102.4000" BottomMargin="37.6000" FontSize="16" LabelText="Read it &#xA;myseft" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="63.0000" Y="40.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="90.0000" Y="57.6000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.3200" />
                        <PreSize X="0.3500" Y="0.2222" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="400.0000" Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="1.0000" Y="0.5000" />
                    <PreSize X="0.4500" Y="0.6000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/button_cover_readmyself.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btreadnarration" ActionTag="-693873134" CallBackType="Click" CallBackName="onReadWithNarration" Tag="34" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="310.0000" RightMargin="-90.0000" TopMargin="60.0000" BottomMargin="60.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="231" Scale9Height="239" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="180.0000" Y="180.0000" />
                    <Children>
                      <AbstractNodeData Name="text_readnarration" ActionTag="-1016619680" Tag="62" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="45.0000" RightMargin="45.0000" TopMargin="98.8000" BottomMargin="41.2000" FontSize="16" LabelText="Record&#xA;your voice" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="90.0000" Y="40.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="90.0000" Y="61.2000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.3400" />
                        <PreSize X="0.5000" Y="0.2222" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="400.0000" Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="1.0000" Y="0.5000" />
                    <PreSize X="0.4500" Y="0.6000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/button_cover_readnarration.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="bt_narration_normal" ActionTag="284753205" VisibleForFrame="False" CallBackType="Click" CallBackName="onNarrationNormal" Tag="65" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="310.0000" RightMargin="-90.0000" TopMargin="60.0000" BottomMargin="60.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="231" Scale9Height="239" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="180.0000" Y="180.0000" />
                    <Children>
                      <AbstractNodeData Name="text_narration_normal" ActionTag="-1581140246" Tag="66" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="50.0000" RightMargin="50.0000" TopMargin="102.4000" BottomMargin="37.6000" FontSize="16" LabelText="With&#xA;narration" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="80.0000" Y="40.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="90.0000" Y="57.6000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.3200" />
                        <PreSize X="0.4444" Y="0.2222" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="400.0000" Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="1.0000" Y="0.5000" />
                    <PreSize X="0.4500" Y="0.6000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/button_cover_narration_normal.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="bt_narration_mute" ActionTag="1809702992" VisibleForFrame="False" CallBackType="Click" CallBackName="onNarrationMute" Tag="63" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="310.0000" RightMargin="-90.0000" TopMargin="60.0000" BottomMargin="60.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="231" Scale9Height="239" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="180.0000" Y="180.0000" />
                    <Children>
                      <AbstractNodeData Name="text_narration_mute" ActionTag="-545111199" Tag="64" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="50.0000" RightMargin="50.0000" TopMargin="102.4000" BottomMargin="37.6000" FontSize="16" LabelText="Mute &#xA;narration" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="80.0000" Y="40.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="90.0000" Y="57.6000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.3200" />
                        <PreSize X="0.4444" Y="0.2222" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="400.0000" Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="1.0000" Y="0.5000" />
                    <PreSize X="0.4500" Y="0.6000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/button_cover_narration_mute.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="450.0000" Y="135.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1929" />
                <PreSize X="0.4444" Y="0.4286" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="btclose" ActionTag="-1503980240" CallBackType="Click" CallBackName="onClose" Tag="24" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="TopEdge" LeftMargin="830.0000" RightMargin="10.0000" TopMargin="5.0000" BottomMargin="635.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="53" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="60.0000" Y="60.0000" />
                <AnchorPoint ScaleX="0.5330" ScaleY="0.4266" />
                <Position X="861.9800" Y="660.5960" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="38" G="176" B="230" />
                <PrePosition X="0.9578" Y="0.9437" />
                <PreSize X="0.0667" Y="0.0857" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_close.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="text_recording" ActionTag="672912588" VisibleForFrame="False" Tag="67" IconVisible="False" VerticalEdge="BottomEdge" LeftMargin="183.0000" RightMargin="283.0000" TopMargin="626.0000" BottomMargin="50.0000" FontSize="20" LabelText="đang ghi âm, trượt sang trái để tiếp tục..." ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="434.0000" Y="24.0000" />
                <AnchorPoint ScaleY="0.5000" />
                <Position X="183.0000" Y="62.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="165" G="42" B="42" />
                <PrePosition X="0.2033" Y="0.0886" />
                <PreSize X="0.4822" Y="0.0343" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBoldItalic.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.8789" Y="0.9115" />
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