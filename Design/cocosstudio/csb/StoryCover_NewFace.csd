<GameFile>
  <PropertyGroup Name="StoryCover_NewFace" Type="Layer" ID="04e115be-4a13-4d17-bcbe-af383e2e1451" Version="3.10.0.0" />
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
      <ObjectData Name="Layer" CustomClassName="StoryInfo_Face" Tag="121" ctype="GameLayerObjectData">
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
          <AbstractNodeData Name="content" ActionTag="-806322282" Tag="212" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="112.0000" RightMargin="112.0000" TopMargin="134.0000" BottomMargin="134.0000" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="800.0000" Y="500.0000" />
            <Children>
              <AbstractNodeData Name="img_book" ActionTag="-1127956774" Tag="23" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" Scale9Enable="True" LeftEage="13" RightEage="13" TopEage="13" BottomEage="13" Scale9OriginX="13" Scale9OriginY="13" Scale9Width="226" Scale9Height="233" ctype="ImageViewObjectData">
                <Size X="800.0000" Y="500.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="400.0000" Y="250.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="1.0000" Y="1.0000" />
                <FileData Type="Normal" Path="mjstory/new_ui/tabsubscribe_planbg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="panel_face" ActionTag="-1797456951" Tag="99" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="125.0000" RightMargin="475.0000" TopMargin="150.0000" BottomMargin="150.0000" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="200.0000" Y="200.0000" />
                <Children>
                  <AbstractNodeData Name="face_mask" ActionTag="-964638979" Alpha="102" CallBackType="Click" CallBackName="onAddFace" Tag="100" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" LeftEage="33" RightEage="33" TopEage="33" BottomEage="33" Scale9OriginX="33" Scale9OriginY="33" Scale9Width="34" Scale9Height="34" ctype="ImageViewObjectData">
                    <Size X="200.0000" Y="200.0000" />
                    <AnchorPoint />
                    <Position />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="1.0000" Y="1.0000" />
                    <FileData Type="Normal" Path="mjstory/new_ui/face_placeholder.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="225.0000" Y="250.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2813" Y="0.5000" />
                <PreSize X="0.2500" Y="0.4000" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="Text_2" ActionTag="-358756041" Tag="54" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="301.0000" RightMargin="301.0000" TopMargin="33.5000" BottomMargin="423.5000" FontSize="35" LabelText="Add a face" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="198.0000" Y="43.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="400.0000" Y="445.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.8900" />
                <PreSize X="0.2475" Y="0.0860" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_3" ActionTag="1761946853" CallBackType="Click" CallBackName="onBack" Tag="98" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="15.0000" RightMargin="705.0000" TopMargin="15.0000" BottomMargin="405.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" Scale9Width="60" Scale9Height="60" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="80.0000" Y="80.0000" />
                <AnchorPoint ScaleY="0.5000" />
                <Position X="15.0000" Y="445.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0188" Y="0.8900" />
                <PreSize X="0.1000" Y="0.1600" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_explore_back.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_1" ActionTag="1628794090" CallBackType="Click" CallBackName="onCamera" Tag="94" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="RightEdge" LeftMargin="440.0000" RightMargin="60.0000" TopMargin="135.0000" BottomMargin="285.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="30" Scale9Height="38" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="300.0000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="1532121810" Tag="56" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="60.0000" RightMargin="180.0000" TopMargin="9.0000" BottomMargin="11.0000" LeftEage="19" RightEage="19" TopEage="19" BottomEage="19" Scale9OriginX="19" Scale9OriginY="19" Scale9Width="22" Scale9Height="22" ctype="ImageViewObjectData">
                    <Size X="60.0000" Y="60.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="90.0000" Y="41.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3000" Y="0.5125" />
                    <PreSize X="0.2000" Y="0.7500" />
                    <FileData Type="Normal" Path="mjstory/new_ui/icon_face_camera.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Text_1" ActionTag="547344558" Tag="96" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="121.0000" RightMargin="79.0000" TopMargin="25.0000" BottomMargin="25.0000" FontSize="25" LabelText="Camera" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="100.0000" Y="30.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="171.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5700" Y="0.5000" />
                    <PreSize X="0.3333" Y="0.3750" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="590.0000" Y="325.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7375" Y="0.6500" />
                <PreSize X="0.3750" Y="0.1600" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_generic_blue.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_1_0" ActionTag="-2143888730" CallBackType="Click" CallBackName="onPhoto" Tag="95" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="RightEdge" LeftMargin="440.0000" RightMargin="60.0000" TopMargin="285.0000" BottomMargin="135.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="30" Scale9Height="38" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="300.0000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="Image_2" ActionTag="1739312125" Tag="57" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="28.0000" RightMargin="212.0000" TopMargin="10.0000" BottomMargin="10.0000" LeftEage="19" RightEage="19" TopEage="19" BottomEage="19" Scale9OriginX="19" Scale9OriginY="19" Scale9Width="22" Scale9Height="22" ctype="ImageViewObjectData">
                    <Size X="60.0000" Y="60.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="58.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1933" Y="0.5000" />
                    <PreSize X="0.2000" Y="0.7500" />
                    <FileData Type="Normal" Path="mjstory/new_ui/icon_face_photo.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Text_1_0" ActionTag="-472141335" Tag="97" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="89.5000" RightMargin="35.5000" TopMargin="25.0000" BottomMargin="25.0000" FontSize="25" LabelText="Photo Library" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="175.0000" Y="30.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="177.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5900" Y="0.5000" />
                    <PreSize X="0.5833" Y="0.3750" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="590.0000" Y="175.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7375" Y="0.3500" />
                <PreSize X="0.3750" Y="0.1600" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_generic_red.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Text_2_0" ActionTag="2045277801" Alpha="102" Tag="55" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="66.5000" RightMargin="66.5000" TopMargin="435.0000" BottomMargin="35.0000" FontSize="25" LabelText="Simply add a face into dozens of personalized books." ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="667.0000" Y="30.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="400.0000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.1000" />
                <PreSize X="0.8338" Y="0.0600" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.7813" Y="0.6510" />
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