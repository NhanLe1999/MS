<GameFile>
  <PropertyGroup Name="MaterialBackWarning" Type="Layer" ID="96e52bd1-12a5-41e4-b452-8ed0de3281d2" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="90" Speed="1.0000" ActivedAnimationName="hide">
        <Timeline ActionTag="-1526135579" Property="Alpha">
          <IntFrame FrameIndex="0" Value="229">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="30" Value="204">
            <EasingData Type="0" />
          </IntFrame>
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
      <ObjectData Name="layer" CustomClassName="MaterialPopup" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="Panel_1" ActionTag="-1526135579" Alpha="229" Tag="93" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="203" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
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
          <AbstractNodeData Name="alert_layout" ActionTag="1153921209" Tag="25" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="310.5000" RightMargin="310.5000" TopMargin="239.9300" BottomMargin="130.0700" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="403.0000" Y="398.0000" />
            <Children>
              <AbstractNodeData Name="alert_bg" ActionTag="521911233" Tag="105" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-97.8552" RightMargin="-99.1448" TopMargin="-175.9888" BottomMargin="-76.0112" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="570" Scale9Height="620" ctype="ImageViewObjectData">
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
                  <AbstractNodeData Name="label_message" ActionTag="100557992" Tag="6" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="60.0000" RightMargin="60.0000" TopMargin="413.9525" BottomMargin="160.0625" IsCustomSize="True" FontSize="25" LabelText="Học liệu đã chọn sẽ mất đi khi ba mẹ thoát ra ngoài." HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="480.0000" Y="75.9850" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="300.0000" Y="198.0550" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="0.5000" Y="0.3047" />
                    <PreSize X="0.8000" Y="0.1169" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="202.1448" Y="248.9888" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5016" Y="0.6256" />
                <PreSize X="1.4888" Y="1.6332" />
                <FileData Type="Normal" Path="mjstory/popup/popup_bg_new.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="exitButton" ActionTag="-243646892" CallBackType="Click" CallBackName="onBackWarningClose" Tag="909" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="-59.0597" RightMargin="263.3404" TopMargin="342.7178" BottomMargin="-33.6310" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="342" Scale9Height="47" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="198.7193" Y="88.9132" />
                <Children>
                  <AbstractNodeData Name="exit_label" ActionTag="-1857739817" Tag="912" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="19.8719" RightMargin="19.8719" TopMargin="8.8913" BottomMargin="8.8913" IsCustomSize="True" FontSize="25" LabelText="Thoát" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="158.9754" Y="71.1306" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="99.3596" Y="44.4566" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.8000" Y="0.8000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="40.3000" Y="10.8256" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1000" Y="0.0272" />
                <PreSize X="0.4931" Y="0.2234" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="mjstory/popup/popup_welcome_bt1_ngang.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/popup/popup_welcome_bt1_ngang.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="continueButton" ActionTag="131232939" CallBackType="Click" CallBackName="onBackWarningContinue" Tag="910" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="140.7453" RightMargin="-60.1453" TopMargin="342.7178" BottomMargin="-33.6310" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="342" Scale9Height="47" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="322.4000" Y="88.9132" />
                <Children>
                  <AbstractNodeData Name="continue_label" ActionTag="-1887062374" Tag="913" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="16.1200" RightMargin="16.1200" TopMargin="8.8913" BottomMargin="8.8913" IsCustomSize="True" FontSize="25" LabelText="Tiếp tục chọn" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="290.1599" Y="71.1306" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="161.2000" Y="44.4566" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.9000" Y="0.8000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="301.9453" Y="10.8256" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7492" Y="0.0272" />
                <PreSize X="0.8000" Y="0.2234" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="mjstory/popup/right_button.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/popup/right_button.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="exclamationIcon" ActionTag="-938499951" Tag="915" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="148.0017" RightMargin="148.0017" TopMargin="60.8144" BottomMargin="243.1780" LeftEage="35" RightEage="35" TopEage="31" BottomEage="31" Scale9OriginX="35" Scale9OriginY="31" Scale9Width="37" Scale9Height="32" ctype="ImageViewObjectData">
                <Size X="106.9965" Y="94.0076" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="201.5000" Y="290.1818" />
                <Scale ScaleX="0.8604" ScaleY="0.8604" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.7291" />
                <PreSize X="0.2655" Y="0.2362" />
                <FileData Type="Normal" Path="mjstory/material/mat_icon_warning_big.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="329.0700" />
            <Scale ScaleX="0.0100" ScaleY="0.0100" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.4285" />
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