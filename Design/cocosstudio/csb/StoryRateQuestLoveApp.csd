<GameFile>
  <PropertyGroup Name="StoryRateQuestLoveApp" Type="Layer" ID="d5dd26de-bfb6-4796-91b7-2697d6b3d864" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="90" Speed="1.0000" ActivedAnimationName="hide">
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
        <Timeline ActionTag="1153921209" Property="Scale">
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
      <ObjectData Name="layer" CustomClassName="StoryRateQuestLoveApp" Tag="31" ctype="GameLayerObjectData">
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
          <AbstractNodeData Name="alert_layout" ActionTag="1153921209" Tag="25" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="310.5000" RightMargin="310.5000" TopMargin="205.9287" BottomMargin="164.0713" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="403.0000" Y="398.0000" />
            <Children>
              <AbstractNodeData Name="alert_bg" ActionTag="521911233" Tag="105" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-52.0000" RightMargin="-52.0000" TopMargin="-123.9946" BottomMargin="-90.0054" LeftEage="152" RightEage="152" TopEage="171" BottomEage="171" Scale9OriginX="152" Scale9OriginY="171" Scale9Width="203" Scale9Height="270" ctype="ImageViewObjectData">
                <Size X="507.0000" Y="612.0000" />
                <Children>
                  <AbstractNodeData Name="label_message" ActionTag="100557992" Tag="6" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="33.5000" RightMargin="33.5000" TopMargin="176.5511" BottomMargin="275.4489" IsCustomSize="True" FontSize="40" LabelText="Do you love our app?" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="440.0000" Y="160.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="253.5000" Y="355.4489" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="40" G="127" B="157" />
                    <PrePosition X="0.5000" Y="0.5808" />
                    <PreSize X="0.8679" Y="0.2614" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btOk" ActionTag="1445712659" CallBackType="Click" CallBackName="onOk" Tag="7" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="70.1000" RightMargin="272.9000" TopMargin="344.1201" BottomMargin="104.8799" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="134" Scale9Height="141" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="164.0000" Y="163.0000" />
                    <Children>
                      <AbstractNodeData Name="label_yes" ActionTag="-2029931772" CallBackType="Click" CallBackName="onOk" Tag="8" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="58.0000" RightMargin="58.0000" TopMargin="171.0000" BottomMargin="-52.0000" TouchEnable="True" FontSize="32" LabelText="Ok" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="48.0000" Y="44.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="82.0000" Y="-30.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="40" G="127" B="157" />
                        <PrePosition X="0.5000" Y="-0.1840" />
                        <PreSize X="0.2927" Y="0.2699" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="152.1000" Y="186.3799" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3000" Y="0.3045" />
                    <PreSize X="0.3235" Y="0.2663" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/theendpage/questlove_yes.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btCancel" ActionTag="794197901" CallBackType="Click" CallBackName="onCancel" Tag="35" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="277.4000" RightMargin="74.6000" TopMargin="345.3107" BottomMargin="103.6893" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="125" Scale9Height="141" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="155.0000" Y="163.0000" />
                    <Children>
                      <AbstractNodeData Name="label_no" ActionTag="-1447711266" CallBackType="Click" CallBackName="onCancel" Tag="36" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="53.5000" RightMargin="53.5000" TopMargin="171.0000" BottomMargin="-52.0000" TouchEnable="True" FontSize="32" LabelText="No" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="48.0000" Y="44.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="77.5000" Y="-30.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="40" G="127" B="157" />
                        <PrePosition X="0.5000" Y="-0.1840" />
                        <PreSize X="0.3097" Y="0.2699" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="354.9000" Y="185.1893" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7000" Y="0.3026" />
                    <PreSize X="0.3057" Y="0.2663" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/theendpage/questlove_no.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="201.5000" Y="215.9946" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5427" />
                <PreSize X="1.2581" Y="1.5377" />
                <FileData Type="Normal" Path="mjstory/theendpage/questlove_bg.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="363.0713" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.4727" />
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