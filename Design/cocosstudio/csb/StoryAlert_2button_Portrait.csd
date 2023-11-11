<GameFile>
  <PropertyGroup Name="StoryAlert_2button_Portrait" Type="Layer" ID="b98efe24-b4b6-453a-b737-93b9530de596" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="90" Speed="1.0000">
        <Timeline ActionTag="974546587" Property="Alpha">
          <IntFrame FrameIndex="0" Value="0">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="30" Value="204">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="60" Value="204">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="90" Value="0">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
        <Timeline ActionTag="794197901" Property="Scale">
          <ScaleFrame FrameIndex="60" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="70" X="0.0100" Y="0.0100">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="1153921209" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.0100" Y="0.0100">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="1.1000" Y="1.1000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="1.1000" Y="1.1000">
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
      <ObjectData Name="layer" CustomClassName="StoryAlert" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="layout" ActionTag="974546587" Alpha="204" Tag="58" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FlipY="True" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="281" Scale9Height="66" ctype="ImageViewObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="-1.0000" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="mjstory/explore/mjsh_explore_bar.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="btcancel" ActionTag="794197901" CallBackType="Click" CallBackName="onCancel" Tag="35" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="20.0000" RightMargin="957.0000" TopMargin="20.0000" BottomMargin="701.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="17" Scale9Height="25" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="47.0000" Y="47.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="43.5000" Y="724.5000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.0425" Y="0.9434" />
            <PreSize X="0.0459" Y="0.0612" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="Normal" Path="mjstory/popup/popup_ratebuy_btclose.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="alert_layout" ActionTag="1153921209" Tag="25" RotationSkewX="-90.0000" RotationSkewY="-90.0000" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="310.5000" RightMargin="310.5000" TopMargin="185.0000" BottomMargin="185.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="403.0000" Y="398.0000" />
            <Children>
              <AbstractNodeData Name="alert_bg" ActionTag="521911233" Tag="105" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-98.5000" RightMargin="-98.5000" TopMargin="-175.9888" BottomMargin="-76.0112" LeftEage="152" RightEage="152" TopEage="171" BottomEage="171" Scale9OriginX="152" Scale9OriginY="171" Scale9Width="296" Scale9Height="308" ctype="ImageViewObjectData">
                <Size X="600.0000" Y="650.0000" />
                <Children>
                  <AbstractNodeData Name="label_title" ActionTag="1749571637" VisibleForFrame="False" Tag="148" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="300.0000" RightMargin="300.0000" TopMargin="270.0000" BottomMargin="380.0000" FontSize="30" LabelText="" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="0.0000" Y="0.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="300.0000" Y="380.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="40" G="127" B="157" />
                    <PrePosition X="0.5000" Y="0.5846" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="label_message" ActionTag="100557992" Tag="6" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="115.0000" RightMargin="115.0000" TopMargin="341.8991" BottomMargin="226.1009" FontSize="30" LabelText="Popup thông báo của &#xA;Monkey Stories bal bla bla " HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="370.0000" Y="82.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="300.0000" Y="267.1009" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="40" G="127" B="157" />
                    <PrePosition X="0.5000" Y="0.4109" />
                    <PreSize X="0.6167" Y="0.1262" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btok" ActionTag="1445712659" CallBackType="Click" CallBackName="onOK" Tag="7" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="39.7385" RightMargin="298.2615" TopMargin="511.4999" BottomMargin="43.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="342" Scale9Height="47" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="262.0000" Y="95.0000" />
                    <Children>
                      <AbstractNodeData Name="label_yes" ActionTag="-2029931772" Tag="8" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="81.0000" RightMargin="81.0000" TopMargin="23.5000" BottomMargin="23.5000" FontSize="35" LabelText="Got it!" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="100.0000" Y="48.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="131.0000" Y="47.5000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.3817" Y="0.5053" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="170.7385" Y="91.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2846" Y="0.1400" />
                    <PreSize X="0.4367" Y="0.1462" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Normal" Path="mjstory/popup/networkdown_ngang_bt1.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/popup/networkdown_ngang_bt1.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btcancel" ActionTag="1285801396" CallBackType="Click" CallBackName="onCancel" Tag="142" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="299.4509" RightMargin="38.5491" TopMargin="511.1135" BottomMargin="43.8865" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="343" Scale9Height="46" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="262.0000" Y="95.0000" />
                    <Children>
                      <AbstractNodeData Name="label_no" ActionTag="-2130823531" Tag="143" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="81.0000" RightMargin="81.0000" TopMargin="23.5000" BottomMargin="23.5000" FontSize="35" LabelText="Got it!" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="100.0000" Y="48.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="131.0000" Y="47.5000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.3817" Y="0.5053" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="430.4509" Y="91.3865" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7174" Y="0.1406" />
                    <PreSize X="0.4367" Y="0.1462" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Normal" Path="mjstory/popup/networkdown_ngang_bt2.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/popup/networkdown_ngang_bt2.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="201.5000" Y="248.9888" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6256" />
                <PreSize X="1.4888" Y="1.6332" />
                <FileData Type="Normal" Path="mjstory/popup/popup_bg.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.1000" ScaleY="1.1000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.3936" Y="0.5182" />
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