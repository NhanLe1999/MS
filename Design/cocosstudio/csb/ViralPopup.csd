<GameFile>
  <PropertyGroup Name="ViralPopup" Type="Layer" ID="3687ba08-2db4-4f0a-9274-dbaf3fbd7880" Version="3.10.0.0" />
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
        <Timeline ActionTag="521911233" Property="Position">
          <PointFrame FrameIndex="0" X="512.0000" Y="445.4400">
            <EasingData Type="26" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="521911233" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="26" />
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
      <ObjectData Name="layer" CustomClassName="ViralPopup" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="layout" ActionTag="-1628935586" Tag="167" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="204" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
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
          <AbstractNodeData Name="holder_layout" ActionTag="1407422652" Tag="154" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="bg_ngang" ActionTag="521911233" Tag="105" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="203.5000" RightMargin="203.5000" TopMargin="34.0600" BottomMargin="156.9400" LeftEage="152" RightEage="152" TopEage="109" BottomEage="109" Scale9OriginX="152" Scale9OriginY="109" Scale9Width="313" Scale9Height="359" ctype="ImageViewObjectData">
                <Size X="617.0000" Y="577.0000" />
                <Children>
                  <AbstractNodeData Name="title_panel" ActionTag="-1533858385" Tag="106" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="208.5000" RightMargin="208.5000" TopMargin="30.3600" BottomMargin="346.6400" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="200.0000" Y="200.0000" />
                    <Children>
                      <AbstractNodeData Name="confetti" ActionTag="-658468964" Tag="107" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-166.5000" RightMargin="-166.5000" BottomMargin="95.0000" LeftEage="175" RightEage="175" TopEage="34" BottomEage="34" Scale9OriginX="175" Scale9OriginY="34" Scale9Width="183" Scale9Height="37" ctype="ImageViewObjectData">
                        <Size X="533.0000" Y="105.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                        <Position X="100.0000" Y="200.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="1.0000" />
                        <PreSize X="2.6650" Y="0.5250" />
                        <FileData Type="Normal" Path="mjstory/popup/viral_popup_confetti.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="title_sub" ActionTag="-831348411" Tag="108" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-141.0000" RightMargin="-141.0000" TopMargin="140.0000" FontSize="25" LabelText="Lets your friends know &#xA;who send this awesome app to them" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="482.0000" Y="60.0000" />
                        <AnchorPoint ScaleX="0.5000" />
                        <Position X="100.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" />
                        <PreSize X="2.4100" Y="0.3000" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="title" ActionTag="625076252" Tag="109" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-107.0000" RightMargin="-107.0000" TopMargin="75.5000" BottomMargin="75.5000" FontSize="40" LabelText="WRITE YOUR NAME" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="414.0000" Y="49.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="100.0000" Y="100.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="2.0700" Y="0.2450" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="308.5000" Y="446.6400" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.7741" />
                    <PreSize X="0.3241" Y="0.3466" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="name_holder" ActionTag="-655678222" Tag="110" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="113.5000" RightMargin="113.5000" TopMargin="288.1200" BottomMargin="218.8800" LeftEage="152" RightEage="152" TopEage="25" BottomEage="25" Scale9OriginX="152" Scale9OriginY="25" Scale9Width="159" Scale9Height="27" ctype="ImageViewObjectData">
                    <Size X="390.0000" Y="70.0000" />
                    <Children>
                      <AbstractNodeData Name="name" ActionTag="1144692304" Tag="111" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="19.5000" RightMargin="19.5000" TopMargin="18.1825" BottomMargin="18.1825" TouchEnable="True" FontSize="22" IsCustomSize="True" LabelText="" PlaceHolderText="Your Name..." MaxLengthEnable="True" MaxLengthText="25" ctype="TextFieldObjectData">
                        <Size X="351.0000" Y="33.6350" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="195.0000" Y="35.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.9000" Y="0.4805" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="308.5000" Y="253.8800" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.4400" />
                    <PreSize X="0.6321" Y="0.1213" />
                    <FileData Type="Normal" Path="mjstory/popup/viral_popup_text.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btclose" ActionTag="1445712659" CallBackType="Click" CallBackName="onClose" Tag="7" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="573.3300" RightMargin="-31.3300" TopMargin="-31.7300" BottomMargin="533.7300" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="53" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="75.0000" Y="75.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="610.8300" Y="571.2300" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9900" Y="0.9900" />
                    <PreSize X="0.1216" Y="0.1300" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/papgemenucontrol_btclose.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btn" ActionTag="-193854663" CallBackType="Click" CallBackName="onSend" Tag="72" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="166.5000" RightMargin="166.5000" TopMargin="405.1000" BottomMargin="58.9000" TouchEnable="True" FontSize="30" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="254" Scale9Height="91" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="284.0000" Y="113.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="308.5000" Y="115.4000" />
                    <Scale ScaleX="0.7500" ScaleY="0.6500" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.2000" />
                    <PreSize X="0.4603" Y="0.1958" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                    <TextColor A="255" R="255" G="255" B="255" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/popup/viral_facebook_btn.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="445.4400" />
                <Scale ScaleX="0.0100" ScaleY="0.0100" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5800" />
                <PreSize X="0.6025" Y="0.7513" />
                <FileData Type="Normal" Path="mjstory/popup/viral_popup_bg.png" Plist="" />
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