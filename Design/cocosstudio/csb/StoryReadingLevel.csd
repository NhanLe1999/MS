<GameFile>
  <PropertyGroup Name="StoryReadingLevel" Type="Layer" ID="9f376536-8ef8-4b00-95ca-f60e56392fbf" Version="3.10.0.0" />
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
        <Timeline ActionTag="231844868" Property="Scale">
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
        <Timeline ActionTag="-765269483" Property="Scale">
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
      <ObjectData Name="layer" CustomClassName="StoryReadingLevel" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="layout" ActionTag="974546587" Alpha="0" CallBackType="Click" CallBackName="onClose" Tag="58" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FlipY="True" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="281" Scale9Height="66" ctype="ImageViewObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="-1.0000" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="mjstory/explore/mjsh_explore_bar.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="layout_welcome" ActionTag="231844868" CallBackType="Click" CallBackName="onClose" Tag="128" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="welcome_doc" ActionTag="-1513937735" VisibleForFrame="False" Tag="134" RotationSkewX="-90.0000" RotationSkewY="-90.0000" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="253.0352" RightMargin="232.9648" TopMargin="-16.0160" BottomMargin="-35.9840" TouchEnable="True" LeftEage="177" RightEage="177" TopEage="178" BottomEage="178" Scale9OriginX="177" Scale9OriginY="178" Scale9Width="184" Scale9Height="464" ctype="ImageViewObjectData">
                <Size X="538.0000" Y="820.0000" />
                <Children>
                  <AbstractNodeData Name="logo" ActionTag="780094760" Tag="135" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="106.5000" RightMargin="106.5000" TopMargin="62.1580" BottomMargin="624.8420" LeftEage="82" RightEage="82" TopEage="40" BottomEage="40" Scale9OriginX="82" Scale9OriginY="40" Scale9Width="161" Scale9Height="53" ctype="ImageViewObjectData">
                    <Size X="325.0000" Y="133.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="269.0000" Y="691.3420" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.8431" />
                    <PreSize X="0.6041" Y="0.1622" />
                    <FileData Type="Normal" Path="mjstory/monkeystories_doc.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="title_lb" ActionTag="832018032" Tag="136" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="78.5000" RightMargin="78.5000" TopMargin="208.7737" BottomMargin="563.2263" FontSize="35" LabelText="Reading Level System" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="381.0000" Y="48.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="269.0000" Y="587.2263" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5000" Y="0.7161" />
                    <PreSize X="0.7082" Y="0.0585" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="content_lb" ActionTag="-607772803" Tag="137" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="217.4906" RightMargin="213.5094" TopMargin="272.8892" BottomMargin="533.1108" FontSize="10" LabelText="Reading Level System" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="107.0000" Y="14.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                    <Position X="270.9906" Y="547.1108" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5037" Y="0.6672" />
                    <PreSize X="0.1989" Y="0.0171" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btnext" ActionTag="112603238" CallBackType="Click" CallBackName="onNext" Tag="138" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="166.0000" RightMargin="166.0000" TopMargin="721.7083" BottomMargin="32.2917" TouchEnable="True" FontSize="30" ButtonText="Next" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="176" Scale9Height="44" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="206.0000" Y="66.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="269.0000" Y="65.2917" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.0796" />
                    <PreSize X="0.3829" Y="0.0805" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <TextColor A="255" R="255" G="255" B="255" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/popup/readinglevel_welcome_bt.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="522.0352" Y="374.0160" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5098" Y="0.4870" />
                <PreSize X="0.5254" Y="1.0677" />
                <FileData Type="Normal" Path="mjstory/popup/readinglevel_welcome_bgdoc.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="0.0100" ScaleY="0.0100" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="layout_ngang" ActionTag="217666867" VisibleForFrame="False" Tag="58" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="94.0000" RightMargin="94.0000" TopMargin="55.0000" BottomMargin="55.0000" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="836.0000" Y="658.0000" />
            <Children>
              <AbstractNodeData Name="layout_touch1" ActionTag="-1005261548" Tag="149" IconVisible="False" LeftMargin="120.1711" RightMargin="115.8289" TopMargin="49.6170" BottomMargin="-26.6170" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="600.0000" Y="635.0000" />
                <AnchorPoint />
                <Position X="120.1711" Y="-26.6170" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1437" Y="-0.0405" />
                <PreSize X="0.7177" Y="0.9650" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="layout_touch2" ActionTag="2008259586" Tag="150" IconVisible="False" LeftMargin="208.7635" RightMargin="212.2365" TopMargin="-30.5065" BottomMargin="608.5065" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="415.0000" Y="80.0000" />
                <AnchorPoint />
                <Position X="208.7635" Y="608.5065" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2497" Y="0.9248" />
                <PreSize X="0.4964" Y="0.1216" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="titlebg" ActionTag="-1801756055" Tag="40" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="184.0000" RightMargin="184.0000" TopMargin="-37.4548" BottomMargin="592.4548" LeftEage="62" RightEage="62" TopEage="33" BottomEage="33" Scale9OriginX="62" Scale9OriginY="33" Scale9Width="344" Scale9Height="37" ctype="ImageViewObjectData">
                <Size X="468.0000" Y="103.0000" />
                <Children>
                  <AbstractNodeData Name="title" ActionTag="-1905429183" Tag="5" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="54.0000" RightMargin="54.0000" TopMargin="18.6792" BottomMargin="50.3208" FontSize="28" LabelText="Monkey Junior reading levels" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="360.0000" Y="34.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="234.0000" Y="67.3208" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="0.5000" Y="0.6536" />
                    <PreSize X="0.7692" Y="0.3301" />
                    <FontResource Type="Normal" Path="fonts/Linotte Semi Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="418.0000" Y="643.9548" />
                <Scale ScaleX="0.9000" ScaleY="0.9000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.9787" />
                <PreSize X="0.5598" Y="0.1565" />
                <FileData Type="Normal" Path="mjstory/popup/readinglevel_titlebg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="bg" ActionTag="521911233" Tag="105" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="114.5000" RightMargin="114.5000" TopMargin="14.5300" BottomMargin="-31.5300" LeftEage="152" RightEage="152" TopEage="171" BottomEage="171" Scale9OriginX="152" Scale9OriginY="171" Scale9Width="303" Scale9Height="333" ctype="ImageViewObjectData">
                <Size X="607.0000" Y="675.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="418.0000" Y="305.9700" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4650" />
                <PreSize X="0.7261" Y="1.0258" />
                <FileData Type="Normal" Path="mjstory/popup/readinglevel_bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="listview_level" ActionTag="1839556609" Tag="42" IconVisible="False" LeftMargin="120.5000" RightMargin="117.5000" TopMargin="11.4945" BottomMargin="606.5055" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="0" ctype="ListViewObjectData">
                <Size X="598.0000" Y="40.0000" />
                <AnchorPoint />
                <Position X="120.5000" Y="606.5055" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1441" Y="0.9217" />
                <PreSize X="0.7153" Y="0.0608" />
                <SingleColor A="255" R="150" G="150" B="255" />
                <FirstColor A="255" R="150" G="150" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="scroll_content" ActionTag="2008387731" Tag="41" IconVisible="False" LeftMargin="122.0426" RightMargin="118.9574" TopMargin="52.6685" BottomMargin="-26.6685" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="Vertical" ctype="ScrollViewObjectData">
                <Size X="595.0000" Y="632.0000" />
                <AnchorPoint />
                <Position X="122.0426" Y="-26.6685" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1460" Y="-0.0405" />
                <PreSize X="0.7117" Y="0.9605" />
                <SingleColor A="255" R="255" G="150" B="100" />
                <FirstColor A="255" R="255" G="150" B="100" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
                <InnerNodeSize Width="595" Height="652" />
              </AbstractNodeData>
              <AbstractNodeData Name="monkeyimg" ActionTag="979317611" Tag="25" IconVisible="False" LeftMargin="661.6500" RightMargin="14.3500" TopMargin="92.5004" BottomMargin="318.4996" LeftEage="52" RightEage="52" TopEage="81" BottomEage="81" Scale9OriginX="52" Scale9OriginY="81" Scale9Width="56" Scale9Height="85" ctype="ImageViewObjectData">
                <Size X="160.0000" Y="247.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="741.6500" Y="441.9996" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8871" Y="0.6717" />
                <PreSize X="0.1914" Y="0.3754" />
                <FileData Type="Normal" Path="mjstory/popup/readinglevel_monkey.png" Plist="" />
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
          <AbstractNodeData Name="layout_doc" ActionTag="-765269483" VisibleForFrame="False" Tag="26" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="94.0000" RightMargin="94.0000" TopMargin="55.0000" BottomMargin="55.0000" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="836.0000" Y="658.0000" />
            <Children>
              <AbstractNodeData Name="layout_touch1" ActionTag="461793068" Tag="27" RotationSkewX="-90.0000" RotationSkewY="-90.0000" IconVisible="False" LeftMargin="874.6521" RightMargin="-638.6521" TopMargin="-218.3089" BottomMargin="31.3089" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="600.0000" Y="845.0000" />
                <AnchorPoint />
                <Position X="874.6521" Y="31.3089" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="1.0462" Y="0.0476" />
                <PreSize X="0.7177" Y="1.2842" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="layout_touch2" ActionTag="1681721521" Tag="28" RotationSkewX="-90.0000" RotationSkewY="-90.0000" IconVisible="False" LeftMargin="29.5804" RightMargin="391.4196" TopMargin="455.7526" BottomMargin="122.2474" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="415.0000" Y="80.0000" />
                <AnchorPoint />
                <Position X="29.5804" Y="122.2474" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0354" Y="0.1858" />
                <PreSize X="0.4964" Y="0.1216" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="titlebg" ActionTag="291786903" Tag="29" RotationSkewX="-90.0000" RotationSkewY="-90.0000" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-237.6784" RightMargin="605.6784" TopMargin="277.5000" BottomMargin="277.5000" LeftEage="62" RightEage="62" TopEage="33" BottomEage="33" Scale9OriginX="62" Scale9OriginY="33" Scale9Width="344" Scale9Height="37" ctype="ImageViewObjectData">
                <Size X="468.0000" Y="103.0000" />
                <Children>
                  <AbstractNodeData Name="title" ActionTag="463330811" Tag="30" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="54.0000" RightMargin="54.0000" TopMargin="18.6792" BottomMargin="50.3208" FontSize="28" LabelText="Monkey Junior reading levels" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="360.0000" Y="34.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="234.0000" Y="67.3208" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="0.5000" Y="0.6536" />
                    <PreSize X="0.7692" Y="0.3301" />
                    <FontResource Type="Normal" Path="fonts/Linotte Semi Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="-3.6784" Y="329.0000" />
                <Scale ScaleX="0.9000" ScaleY="0.9000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="-0.0044" Y="0.5000" />
                <PreSize X="0.5598" Y="0.1565" />
                <FileData Type="Normal" Path="mjstory/popup/readinglevel_titlebg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="bg" ActionTag="563988836" Tag="31" RotationSkewX="-90.0000" RotationSkewY="-90.0000" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="132.4740" RightMargin="96.5260" TopMargin="-121.0000" BottomMargin="-121.0000" LeftEage="152" RightEage="152" TopEage="171" BottomEage="171" Scale9OriginX="152" Scale9OriginY="171" Scale9Width="303" Scale9Height="333" ctype="ImageViewObjectData">
                <Size X="607.0000" Y="900.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="435.9740" Y="329.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5215" Y="0.5000" />
                <PreSize X="0.7261" Y="1.3678" />
                <FileData Type="Normal" Path="mjstory/popup/readinglevel_bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="listview_level" ActionTag="-565378126" Tag="32" IconVisible="False" LeftMargin="-24.3239" RightMargin="800.3239" TopMargin="28.5561" BottomMargin="31.4439" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="0" DirectionType="Vertical" ctype="ListViewObjectData">
                <Size X="60.0000" Y="598.0000" />
                <AnchorPoint ScaleX="1.0000" />
                <Position X="35.6761" Y="31.4439" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0427" Y="0.0478" />
                <PreSize X="0.0718" Y="0.9088" />
                <SingleColor A="255" R="150" G="150" B="255" />
                <FirstColor A="255" R="150" G="150" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="scroll_content" ActionTag="1023671972" Tag="33" IconVisible="False" LeftMargin="37.0629" RightMargin="-43.0629" TopMargin="30.5239" BottomMargin="32.4761" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="Horizontal" ctype="ScrollViewObjectData">
                <Size X="842.0000" Y="595.0000" />
                <AnchorPoint />
                <Position X="37.0629" Y="32.4761" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0443" Y="0.0494" />
                <PreSize X="1.0072" Y="0.9043" />
                <SingleColor A="255" R="255" G="150" B="100" />
                <FirstColor A="255" R="255" G="150" B="100" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
                <InnerNodeSize Width="842" Height="595" />
              </AbstractNodeData>
              <AbstractNodeData Name="monkeyimg" ActionTag="102207454" Tag="34" RotationSkewX="-90.0000" RotationSkewY="-90.0000" IconVisible="False" LeftMargin="218.4488" RightMargin="457.5512" TopMargin="-111.9794" BottomMargin="522.9794" LeftEage="52" RightEage="52" TopEage="81" BottomEage="81" Scale9OriginX="52" Scale9OriginY="81" Scale9Width="56" Scale9Height="85" ctype="ImageViewObjectData">
                <Size X="160.0000" Y="247.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="298.4488" Y="646.4794" />
                <Scale ScaleX="0.7500" ScaleY="0.7500" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3570" Y="0.9825" />
                <PreSize X="0.1914" Y="0.3754" />
                <FileData Type="Normal" Path="mjstory/popup/readinglevel_monkey.png" Plist="" />
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
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>