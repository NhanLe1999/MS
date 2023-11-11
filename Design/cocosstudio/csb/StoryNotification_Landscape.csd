<GameFile>
  <PropertyGroup Name="StoryNotification_Landscape" Type="Layer" ID="07e565e0-de92-4939-a87d-82eb81d80e19" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="80" Speed="1.0000">
        <Timeline ActionTag="2120476648" Property="Alpha">
          <IntFrame FrameIndex="0" Value="0">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="30" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="50" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="80" Value="0">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
        <Timeline ActionTag="-1872250133" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.0100" Y="0.0100">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="1.0000" Y="1.0000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="50" X="1.0000" Y="1.0000">
            <EasingData Type="25" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="80" X="0.0100" Y="0.0100">
            <EasingData Type="25" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="show" StartIndex="0" EndIndex="35">
          <RenderColor A="255" R="255" G="255" B="0" />
        </AnimationInfo>
        <AnimationInfo Name="hide" StartIndex="50" EndIndex="85">
          <RenderColor A="255" R="176" G="196" B="222" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="StoryNotification" Tag="44" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="layout_touch" ActionTag="2120476648" Alpha="178" Tag="17" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="204" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="khung1" ActionTag="-1872250133" Tag="18" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="313.1980" RightMargin="339.8020" TopMargin="25.1264" BottomMargin="662.8736" LeftEage="122" RightEage="122" TopEage="26" BottomEage="26" Scale9OriginX="122" Scale9OriginY="26" Scale9Width="127" Scale9Height="28" ctype="ImageViewObjectData">
            <Size X="371.0000" Y="80.0000" />
            <Children>
              <AbstractNodeData Name="title" ActionTag="423677807" Tag="21" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="81.0000" RightMargin="81.0000" TopMargin="19.0000" BottomMargin="19.0000" FontSize="35" LabelText="Notifications" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="209.0000" Y="42.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="185.5000" Y="40.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.5633" Y="0.5250" />
                <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="khung2" ActionTag="32516515" Tag="19" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="-108.0000" RightMargin="-108.0000" TopMargin="71.9995" BottomMargin="-734.9995" LeftEage="193" RightEage="193" TopEage="245" BottomEage="245" Scale9OriginX="193" Scale9OriginY="245" Scale9Width="201" Scale9Height="253" ctype="ImageViewObjectData">
                <Size X="587.0000" Y="743.0000" />
                <Children>
                  <AbstractNodeData Name="content_scroll" ActionTag="-12403164" Tag="46" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-19.0000" RightMargin="-19.0000" TopMargin="11.5165" BottomMargin="11.4835" TouchEnable="True" ClipAble="True" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" IsBounceEnabled="True" ScrollDirectionType="Vertical" ctype="ScrollViewObjectData">
                    <Size X="625.0000" Y="720.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                    <Position X="293.5000" Y="731.4835" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.9845" />
                    <PreSize X="1.0647" Y="0.9690" />
                    <SingleColor A="255" R="255" G="255" B="255" />
                    <FirstColor A="255" R="255" G="150" B="100" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                    <InnerNodeSize Width="625" Height="720" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="185.5000" Y="8.0005" />
                <Scale ScaleX="0.8500" ScaleY="0.8500" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1000" />
                <PreSize X="1.5822" Y="9.2875" />
                <FileData Type="Normal" Path="mjstory/popup/noti_khung1.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="btclose" ActionTag="1586702608" CallBackType="Click" CallBackName="onClose" Tag="20" IconVisible="False" LeftMargin="395.1179" RightMargin="-97.1179" TopMargin="35.8547" BottomMargin="-32.8547" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="43" Scale9Height="55" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="73.0000" Y="77.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="431.6179" Y="5.6453" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="1.1634" Y="0.0706" />
                <PreSize X="0.1968" Y="0.9625" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/popup/noti_btclose.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="498.6980" Y="702.8736" />
            <Scale ScaleX="1.0794" ScaleY="1.0794" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.4870" Y="0.9152" />
            <PreSize X="0.3623" Y="0.1042" />
            <FileData Type="Normal" Path="mjstory/popup/noti_khung2.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>