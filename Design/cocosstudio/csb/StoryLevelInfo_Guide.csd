<GameFile>
  <PropertyGroup Name="StoryLevelInfo_Guide" Type="Layer" ID="dfc7c78a-2e97-40ba-b8e7-a4bb43fe9295" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="90" Speed="1.0000">
        <Timeline ActionTag="521911233" Property="Scale">
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
        <Timeline ActionTag="595762391" Property="Scale">
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
      <ObjectData Name="layer" CustomClassName="StoryLevelInfo_Guide" Tag="31" ctype="GameLayerObjectData">
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
          <AbstractNodeData Name="bg_ngang" ActionTag="521911233" VisibleForFrame="False" Tag="105" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="BottomEdge" LeftMargin="755.8146" RightMargin="61.1854" TopMargin="446.6639" BottomMargin="67.3361" LeftEage="68" RightEage="68" TopEage="83" BottomEage="83" Scale9OriginX="68" Scale9OriginY="83" Scale9Width="71" Scale9Height="88" ctype="ImageViewObjectData">
            <Size X="207.0000" Y="254.0000" />
            <Children>
              <AbstractNodeData Name="title" ActionTag="-2138273510" Tag="4" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="31.1910" RightMargin="25.8090" TopMargin="25.0600" BottomMargin="206.9400" FontSize="18" LabelText="Monkey Junior Tip" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="150.0000" Y="22.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="106.1910" Y="217.9400" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="26" G="26" B="26" />
                <PrePosition X="0.5130" Y="0.8580" />
                <PreSize X="0.7246" Y="0.0866" />
                <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="content" ActionTag="100557992" Tag="6" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="20.8359" RightMargin="15.1641" TopMargin="49.5210" BottomMargin="150.4790" FontSize="15" LabelText="To find out more about&#xA;Monkey Junior's Reading&#xA;Levels tap this button" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="171.0000" Y="54.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="106.3359" Y="177.4790" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="26" G="26" B="26" />
                <PrePosition X="0.5137" Y="0.6987" />
                <PreSize X="0.8261" Y="0.2126" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="level_info" ActionTag="2022118366" CallBackType="Click" CallBackName="onLevelInfo" Tag="74" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="173.3962" RightMargin="-61.3962" TopMargin="225.1826" BottomMargin="-66.1826" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="95.0000" Y="95.0000" />
                <AnchorPoint ScaleX="1.0000" />
                <Position X="268.3962" Y="-66.1826" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="1.2966" Y="-0.2606" />
                <PreSize X="0.4589" Y="0.3740" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="1.0000" />
            <Position X="962.8146" Y="67.3361" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.9402" Y="0.0877" />
            <PreSize X="0.2021" Y="0.3307" />
            <FileData Type="Normal" Path="mjstory/popup/levelinfostory_guide_ngang.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="layout_doc" ActionTag="1118915907" VisibleForFrame="False" Tag="107" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-42.0000" RightMargin="-42.0000" TopMargin="-5.5000" BottomMargin="-5.5000" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1108.0000" Y="779.0000" />
            <Children>
              <AbstractNodeData Name="bg_doc" ActionTag="595762391" Tag="75" RotationSkewX="-90.0000" RotationSkewY="-90.0000" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="BottomEdge" LeftMargin="194.9714" RightMargin="650.0286" TopMargin="464.5512" BottomMargin="108.4488" LeftEage="68" RightEage="68" TopEage="83" BottomEage="83" Scale9OriginX="68" Scale9OriginY="83" Scale9Width="127" Scale9Height="40" ctype="ImageViewObjectData">
                <Size X="263.0000" Y="206.0000" />
                <Children>
                  <AbstractNodeData Name="title" ActionTag="256282420" Tag="76" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="87.0618" RightMargin="29.9382" TopMargin="27.6873" BottomMargin="156.3127" FontSize="18" LabelText="Monkey Junior Tip" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="146.0000" Y="22.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="160.0618" Y="167.3127" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="0.6086" Y="0.8122" />
                    <PreSize X="0.5551" Y="0.1068" />
                    <FontResource Type="Normal" Path="fonts/Linotte Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="content" ActionTag="1752442779" Tag="77" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="75.8254" RightMargin="20.1746" TopMargin="52.4239" BottomMargin="99.5761" FontSize="15" LabelText="To find out more about&#xA;Monkey Junior's Reading&#xA;Levels tap this button" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="167.0000" Y="54.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="159.3254" Y="126.5761" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="0.6058" Y="0.6144" />
                    <PreSize X="0.6350" Y="0.2621" />
                    <FontResource Type="Normal" Path="fonts/Linotte Light.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="level_info" ActionTag="2042940266" CallBackType="Click" CallBackName="onLevelInfo" Tag="78" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="-103.6746" RightMargin="206.6746" TopMargin="-47.2494" BottomMargin="183.2494" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="160.0000" Y="70.0000" />
                    <AnchorPoint />
                    <Position X="-103.6746" Y="183.2494" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="-0.3942" Y="0.8896" />
                    <PreSize X="0.6084" Y="0.3398" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="194.9714" Y="108.4488" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1760" Y="0.1392" />
                <PreSize X="0.2374" Y="0.2644" />
                <FileData Type="Normal" Path="mjstory/popup/levelinfostory_guide_doc.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0820" Y="1.0143" />
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