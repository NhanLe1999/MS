<GameFile>
  <PropertyGroup Name="StoryLevelInfo" Type="Layer" ID="78e232bd-8dfc-4bc6-84e1-b669f16d4180" Version="3.10.0.0" />
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
        <Timeline ActionTag="217666867" Property="Scale">
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
        <Timeline ActionTag="1875609364" Property="Scale">
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
      <ObjectData Name="layer" CustomClassName="StoryLevelInfo" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="layout" ActionTag="974546587" Alpha="0" Tag="58" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FlipY="True" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="281" Scale9Height="66" ctype="ImageViewObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="-1.0000" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="mjstory/explore/mjsh_explore_bar.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="layout_ngang" ActionTag="217666867" VisibleForFrame="False" Tag="58" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="94.0000" RightMargin="94.0000" TopMargin="55.0000" BottomMargin="55.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="836.0000" Y="658.0000" />
            <Children>
              <AbstractNodeData Name="bg" ActionTag="521911233" Tag="105" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftEage="152" RightEage="152" TopEage="171" BottomEage="171" Scale9OriginX="152" Scale9OriginY="171" Scale9Width="532" Scale9Height="316" ctype="ImageViewObjectData">
                <Size X="836.0000" Y="658.0000" />
                <Children>
                  <AbstractNodeData Name="thumb" ActionTag="-1801756055" Tag="40" IconVisible="False" LeftMargin="138.3293" RightMargin="508.6707" TopMargin="46.9511" BottomMargin="360.0489" LeftEage="62" RightEage="62" TopEage="82" BottomEage="82" Scale9OriginX="62" Scale9OriginY="82" Scale9Width="65" Scale9Height="87" ctype="ImageViewObjectData">
                    <Size X="189.0000" Y="251.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="232.8293" Y="485.5489" />
                    <Scale ScaleX="0.9000" ScaleY="0.9000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2785" Y="0.7379" />
                    <PreSize X="0.2261" Y="0.3815" />
                    <FileData Type="Normal" Path="mjstory/home/mjsh_stories_thumb_1.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="layout_story" ActionTag="-531208025" Tag="34" IconVisible="False" LeftMargin="421.0000" RightMargin="55.0000" TopMargin="78.9912" BottomMargin="399.0088" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="360.0000" Y="180.0000" />
                    <Children>
                      <AbstractNodeData Name="title" ActionTag="-1905429183" Tag="5" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="92.5000" RightMargin="92.5000" TopMargin="29.9980" BottomMargin="116.0020" FontSize="28" LabelText="Name Story" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="175.0000" Y="34.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="180.0000" Y="133.0020" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.5000" Y="0.7389" />
                        <PreSize X="0.4861" Y="0.1889" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="lexile_lb" ActionTag="100557992" Tag="6" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="58.0680" RightMargin="180.9320" TopMargin="74.0040" BottomMargin="81.9960" FontSize="20" LabelText="Lexile Score" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="121.0000" Y="24.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="58.0680" Y="93.9960" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.1613" Y="0.5222" />
                        <PreSize X="0.3361" Y="0.1333" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="word_lb" ActionTag="380513042" Tag="35" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="58.0680" RightMargin="177.9320" TopMargin="101.2020" BottomMargin="54.7980" FontSize="20" LabelText="Word count" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="124.0000" Y="24.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="58.0680" Y="66.7980" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.1613" Y="0.3711" />
                        <PreSize X="0.3444" Y="0.1333" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="page_lb" ActionTag="-833506708" Tag="36" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="58.0680" RightMargin="182.9320" TopMargin="128.4000" BottomMargin="27.6000" FontSize="20" LabelText="Page count" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="119.0000" Y="24.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="58.0680" Y="39.6000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.1613" Y="0.2200" />
                        <PreSize X="0.3306" Y="0.1333" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="lexile_value_lb" ActionTag="-1530236067" Tag="37" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="177.3240" RightMargin="53.6760" TopMargin="74.0040" BottomMargin="81.9960" FontSize="20" LabelText="400L - 500L" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="129.0000" Y="24.0000" />
                        <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                        <Position X="306.3240" Y="93.9960" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.8509" Y="0.5222" />
                        <PreSize X="0.3583" Y="0.1333" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="word_value_lb" ActionTag="1288331772" Tag="38" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="266.3240" RightMargin="53.6760" TopMargin="101.2020" BottomMargin="54.7980" FontSize="20" LabelText="793" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="40.0000" Y="24.0000" />
                        <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                        <Position X="306.3240" Y="66.7980" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.8509" Y="0.3711" />
                        <PreSize X="0.1111" Y="0.1333" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="page_value_lb" ActionTag="689185336" Tag="39" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="283.3240" RightMargin="53.6760" TopMargin="128.4000" BottomMargin="27.6000" FontSize="20" LabelText="12" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="23.0000" Y="24.0000" />
                        <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                        <Position X="306.3240" Y="39.6000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.8509" Y="0.2200" />
                        <PreSize X="0.0639" Y="0.1333" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="421.0000" Y="399.0088" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5036" Y="0.6064" />
                    <PreSize X="0.4306" Y="0.2736" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btclose" ActionTag="1445712659" CallBackType="Click" CallBackName="onClose" Tag="7" IconVisible="False" LeftMargin="793.0127" RightMargin="-17.0127" TopMargin="-14.7457" BottomMargin="614.7457" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="30" Scale9Height="36" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="60.0000" Y="58.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="823.0127" Y="643.7457" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9845" Y="0.9783" />
                    <PreSize X="0.0718" Y="0.0881" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/popup/levelinfostory_btclose.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="418.0000" Y="329.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="1.0000" Y="1.0000" />
                <FileData Type="Normal" Path="mjstory/popup/levelinfostory_bgngang.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="listview_level" ActionTag="1839556609" Tag="42" IconVisible="False" LeftMargin="40.4999" RightMargin="43.5001" TopMargin="303.4964" BottomMargin="294.5036" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="0" ctype="ListViewObjectData">
                <Size X="752.0000" Y="60.0000" />
                <AnchorPoint />
                <Position X="40.4999" Y="294.5036" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0484" Y="0.4476" />
                <PreSize X="0.8995" Y="0.0912" />
                <SingleColor A="255" R="150" G="150" B="255" />
                <FirstColor A="255" R="150" G="150" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="scroll_content" ActionTag="2008387731" Tag="41" IconVisible="False" LeftMargin="42.1504" RightMargin="43.8496" TopMargin="364.9433" BottomMargin="41.0567" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="Vertical" ctype="ScrollViewObjectData">
                <Size X="750.0000" Y="252.0000" />
                <AnchorPoint />
                <Position X="42.1504" Y="41.0567" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0504" Y="0.0624" />
                <PreSize X="0.8971" Y="0.3830" />
                <SingleColor A="255" R="255" G="150" B="100" />
                <FirstColor A="255" R="255" G="150" B="100" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
                <InnerNodeSize Width="750" Height="252" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="0.0100" ScaleY="0.0100" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.8164" Y="0.8568" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="layout_doc" ActionTag="1875609364" VisibleForFrame="False" Tag="44" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="181.0000" RightMargin="181.0000" TopMargin="72.0000" BottomMargin="72.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="662.0000" Y="624.0000" />
            <Children>
              <AbstractNodeData Name="bg" ActionTag="-816736926" Tag="31" RotationSkewX="-90.0000" RotationSkewY="-90.0000" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="19.0000" RightMargin="19.0000" TopMargin="-19.0000" BottomMargin="-19.0000" LeftEage="152" RightEage="152" TopEage="171" BottomEage="171" Scale9OriginX="152" Scale9OriginY="171" Scale9Width="320" Scale9Height="320" ctype="ImageViewObjectData">
                <Size X="624.0000" Y="662.0000" />
                <Children>
                  <AbstractNodeData Name="thumb" ActionTag="-1318711903" Tag="32" IconVisible="False" LeftMargin="57.2412" RightMargin="377.7588" TopMargin="41.5346" BottomMargin="369.4654" LeftEage="62" RightEage="62" TopEage="82" BottomEage="82" Scale9OriginX="62" Scale9OriginY="82" Scale9Width="65" Scale9Height="87" ctype="ImageViewObjectData">
                    <Size X="189.0000" Y="251.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="151.7412" Y="494.9654" />
                    <Scale ScaleX="0.9000" ScaleY="0.9000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2432" Y="0.7477" />
                    <PreSize X="0.3029" Y="0.3792" />
                    <FileData Type="Normal" Path="mjstory/home/mjsh_stories_thumb_1.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="layout_story" ActionTag="-1499357487" Tag="33" IconVisible="False" LeftMargin="313.2440" RightMargin="30.7560" TopMargin="79.4487" BottomMargin="412.5513" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="280.0000" Y="170.0000" />
                    <Children>
                      <AbstractNodeData Name="title" ActionTag="405427317" Tag="34" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="61.5000" RightMargin="61.5000" TopMargin="27.5000" BottomMargin="112.5000" FontSize="25" LabelText="Name Story" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="157.0000" Y="30.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="140.0000" Y="127.5000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.5000" Y="0.7500" />
                        <PreSize X="0.5607" Y="0.1765" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="lexile_lb" ActionTag="195817367" Tag="35" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="27.8320" RightMargin="131.1680" TopMargin="69.2260" BottomMargin="76.7740" FontSize="20" LabelText="Lexile Score" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="121.0000" Y="24.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="27.8320" Y="88.7740" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.0994" Y="0.5222" />
                        <PreSize X="0.4321" Y="0.1412" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="word_lb" ActionTag="845635071" Tag="36" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="27.8320" RightMargin="128.1680" TopMargin="94.9130" BottomMargin="51.0870" FontSize="20" LabelText="Word count" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="124.0000" Y="24.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="27.8320" Y="63.0870" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.0994" Y="0.3711" />
                        <PreSize X="0.4429" Y="0.1412" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="page_lb" ActionTag="1504123273" Tag="37" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="27.8320" RightMargin="133.1680" TopMargin="120.6000" BottomMargin="25.4000" FontSize="20" LabelText="Page count" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="119.0000" Y="24.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="27.8320" Y="37.4000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.0994" Y="0.2200" />
                        <PreSize X="0.4250" Y="0.1412" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="lexile_value_lb" ActionTag="1269360113" Tag="38" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="148.7040" RightMargin="37.2960" TopMargin="72.2260" BottomMargin="79.7740" FontSize="15" LabelText="400L - 500L" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="94.0000" Y="18.0000" />
                        <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                        <Position X="242.7040" Y="88.7740" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.8668" Y="0.5222" />
                        <PreSize X="0.3357" Y="0.1059" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="word_value_lb" ActionTag="-173825996" Tag="39" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="202.7040" RightMargin="37.2960" TopMargin="94.9130" BottomMargin="51.0870" FontSize="20" LabelText="793" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="40.0000" Y="24.0000" />
                        <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                        <Position X="242.7040" Y="63.0870" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.8668" Y="0.3711" />
                        <PreSize X="0.1429" Y="0.1412" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="page_value_lb" ActionTag="2007617826" Tag="40" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="219.7040" RightMargin="37.2960" TopMargin="120.6000" BottomMargin="25.4000" FontSize="20" LabelText="12" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="23.0000" Y="24.0000" />
                        <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                        <Position X="242.7040" Y="37.4000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.8668" Y="0.2200" />
                        <PreSize X="0.0821" Y="0.1412" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="313.2440" Y="412.5513" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5020" Y="0.6232" />
                    <PreSize X="0.4487" Y="0.2568" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btclose" ActionTag="-715905544" CallBackType="Click" CallBackName="onClose" Tag="41" IconVisible="False" LeftMargin="585.6462" RightMargin="-21.6462" TopMargin="-18.8956" BottomMargin="622.8956" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="30" Scale9Height="36" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="60.0000" Y="58.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="615.6462" Y="651.8956" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9866" Y="0.9847" />
                    <PreSize X="0.0962" Y="0.0876" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/popup/levelinfostory_btclose.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="331.0000" Y="312.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.9426" Y="1.0609" />
                <FileData Type="Normal" Path="mjstory/popup/levelinfostory_bgdoc.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="scroll_content" ActionTag="1359487570" Tag="43" IconVisible="False" LeftMargin="358.9510" RightMargin="21.0490" TopMargin="14.4520" BottomMargin="17.5480" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="Horizontal" ctype="ScrollViewObjectData">
                <Size X="282.0000" Y="592.0000" />
                <AnchorPoint />
                <Position X="358.9510" Y="17.5480" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5422" Y="0.0281" />
                <PreSize X="0.4260" Y="0.9487" />
                <SingleColor A="255" R="255" G="150" B="100" />
                <FirstColor A="255" R="255" G="150" B="100" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
                <InnerNodeSize Width="282" Height="592" />
              </AbstractNodeData>
              <AbstractNodeData Name="listview_level" ActionTag="-214231027" Tag="42" IconVisible="False" LeftMargin="297.9500" RightMargin="304.0500" TopMargin="15.2499" BottomMargin="16.7501" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="0" DirectionType="Vertical" ctype="ListViewObjectData">
                <Size X="60.0000" Y="592.0000" />
                <AnchorPoint ScaleX="1.0000" />
                <Position X="357.9500" Y="16.7501" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5407" Y="0.0268" />
                <PreSize X="0.0906" Y="0.9487" />
                <SingleColor A="255" R="150" G="150" B="255" />
                <FirstColor A="255" R="150" G="150" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="0.0100" ScaleY="0.0100" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.6465" Y="0.8125" />
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