<GameFile>
  <PropertyGroup Name="game_word_machine" Type="Layer" ID="c455e314-a885-46c0-af05-21cf2a853033" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="110" Speed="1.0000" ActivedAnimationName="unload">
        <Timeline ActionTag="1086541244" Property="Scale">
          <ScaleFrame FrameIndex="10" X="0.0010" Y="1.0000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="50" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="1.0000" Y="1.0000">
            <EasingData Type="25" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="100" X="0.0010" Y="1.0000">
            <EasingData Type="26" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="877301918" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.0010" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="20" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="90" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="110" X="0.0010" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="load" StartIndex="0" EndIndex="50">
          <RenderColor A="150" R="199" G="21" B="133" />
        </AnimationInfo>
        <AnimationInfo Name="unload" StartIndex="60" EndIndex="110">
          <RenderColor A="150" R="255" G="0" B="255" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="MJGameWordMachine" Tag="127" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="background" ActionTag="-833539902" Tag="128" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-188.0000" RightMargin="-188.0000" LeftEage="462" RightEage="462" TopEage="253" BottomEage="253" Scale9OriginX="462" Scale9OriginY="253" Scale9Width="476" Scale9Height="262" ctype="ImageViewObjectData">
            <Size X="1400.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.3672" Y="1.0000" />
            <FileData Type="Normal" Path="games/word_machine/background.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="content" ActionTag="-1350312477" Tag="129" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="logo" ActionTag="398513691" Tag="285" IconVisible="False" LeftMargin="439.0000" RightMargin="113.0000" TopMargin="210.0000" BottomMargin="242.0000" LeftEage="155" RightEage="155" TopEage="104" BottomEage="104" Scale9OriginX="155" Scale9OriginY="104" Scale9Width="162" Scale9Height="108" ctype="ImageViewObjectData">
                <Size X="472.0000" Y="316.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="675.0000" Y="400.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.6592" Y="0.5208" />
                <PreSize X="0.4609" Y="0.4115" />
                <FileData Type="Normal" Path="games/word_machine/logo.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Panel_4" ActionTag="877301918" Tag="204" IconVisible="False" LeftMargin="255.0000" RightMargin="569.0000" TopMargin="313.0000" BottomMargin="255.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                <Size X="200.0000" Y="200.0000" />
                <Children>
                  <AbstractNodeData Name="Image_2_0" ActionTag="-904922697" Tag="131" IconVisible="False" TopMargin="20.0000" LeftEage="66" RightEage="66" TopEage="59" BottomEage="59" Scale9OriginX="66" Scale9OriginY="59" Scale9Width="68" Scale9Height="62" ctype="ImageViewObjectData">
                    <Size X="200.0000" Y="180.0000" />
                    <AnchorPoint />
                    <Position />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="1.0000" Y="0.9000" />
                    <FileData Type="Normal" Path="games/word_machine/stick.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="frame" ActionTag="1086541244" Tag="130" IconVisible="False" HorizontalEdge="RightEdge" LeftMargin="150.0000" RightMargin="-550.0000" TopMargin="-225.0000" BottomMargin="-25.0000" Scale9Enable="True" LeftEage="30" RightEage="30" TopEage="30" BottomEage="30" Scale9OriginX="30" Scale9OriginY="30" Scale9Width="540" Scale9Height="290" ctype="ImageViewObjectData">
                    <Size X="600.0000" Y="450.0000" />
                    <Children>
                      <AbstractNodeData Name="image" ActionTag="-752450802" Tag="140" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="64.0000" RightMargin="64.0000" TopMargin="67.0000" BottomMargin="67.0000" LeftEage="157" RightEage="157" TopEage="86" BottomEage="86" Scale9OriginX="157" Scale9OriginY="86" Scale9Width="158" Scale9Height="144" ctype="ImageViewObjectData">
                        <Size X="472.0000" Y="316.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="300.0000" Y="225.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.7867" Y="0.7022" />
                        <FileData Type="Normal" Path="games/word_machine/logo.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="150.0000" Y="200.0000" />
                    <Scale ScaleX="0.0010" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7500" Y="1.0000" />
                    <PreSize X="3.0000" Y="2.2500" />
                    <FileData Type="Normal" Path="games/word_machine/frame.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="255.0000" Y="255.0000" />
                <Scale ScaleX="0.0010" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2490" Y="0.3320" />
                <PreSize X="0.1953" Y="0.2604" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="Panel_2" ActionTag="-1102854723" Tag="136" IconVisible="False" LeftMargin="-140.0000" RightMargin="614.0000" TopMargin="83.0000" BottomMargin="85.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="550.0000" Y="600.0000" />
                <Children>
                  <AbstractNodeData Name="pipe_structure" ActionTag="165585677" Tag="29" IconVisible="True" LeftMargin="258.5000" RightMargin="166.5000" TopMargin="454.0000" BottomMargin="66.0000" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
                    <Size X="125.0000" Y="80.0000" />
                    <AnchorPoint />
                    <Position X="258.5000" Y="66.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4700" Y="0.1100" />
                    <PreSize X="0.2273" Y="0.1333" />
                    <FileData Type="Normal" Path="csb/game/word_machine/pipe.csd" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="machine" ActionTag="1182581338" Tag="134" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="BottomEdge" LeftMargin="5.9999" RightMargin="-5.9999" TopMargin="50.0001" BottomMargin="-0.0001" LeftEage="181" RightEage="181" TopEage="181" BottomEage="181" Scale9OriginX="181" Scale9OriginY="181" Scale9Width="188" Scale9Height="188" ctype="ImageViewObjectData">
                    <Size X="550.0000" Y="550.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="280.9999" Y="274.9999" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5109" Y="0.4583" />
                    <PreSize X="1.0000" Y="0.9167" />
                    <FileData Type="Normal" Path="games/word_machine/machine.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="filter" ActionTag="-1041572982" Tag="135" IconVisible="False" LeftMargin="182.5000" RightMargin="92.5000" TopMargin="34.4198" BottomMargin="365.5802" LeftEage="90" RightEage="90" TopEage="66" BottomEage="66" Scale9OriginX="90" Scale9OriginY="66" Scale9Width="95" Scale9Height="68" ctype="ImageViewObjectData">
                    <Size X="275.0000" Y="200.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="320.0000" Y="465.5802" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5818" Y="0.7760" />
                    <PreSize X="0.5000" Y="0.3333" />
                    <FileData Type="Normal" Path="games/word_machine/filter.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_holder" ActionTag="970810268" Tag="139" IconVisible="False" VerticalEdge="TopEdge" LeftMargin="-166.9999" RightMargin="-257.0001" TopMargin="-49.9996" BottomMargin="589.9996" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="974.0000" Y="60.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="320.0001" Y="619.9996" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5818" Y="1.0333" />
                    <PreSize X="1.7709" Y="0.1000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="-140.0000" Y="85.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="-0.1367" Y="0.1107" />
                <PreSize X="0.5371" Y="0.7813" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
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