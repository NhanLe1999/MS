<GameFile>
  <PropertyGroup Name="StoryRatePopUp" Type="Layer" ID="a6c38196-4b22-4c37-a302-eef0dd31fd40" Version="3.10.0.0" />
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
        <Timeline ActionTag="-1636865244" Property="Scale">
          <ScaleFrame FrameIndex="60" X="1.2000" Y="1.2000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="70" X="0.0100" Y="0.0100">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="521911233" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.0100" Y="0.0100">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="0.9000" Y="0.9000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="0.9000" Y="0.9000">
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
      <ObjectData Name="layer" CustomClassName="StoryRatePopUp" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="layout" ActionTag="974546587" Alpha="216" CallBackName="onClose" Tag="58" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FlipY="True" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="281" Scale9Height="66" ctype="ImageViewObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="-1.0000" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="mjstory/explore/mjsh_explore_bar.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="btclose" ActionTag="-1636865244" CallBackType="Click" CallBackName="onClose" Tag="19" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="TopEdge" LeftMargin="959.2913" RightMargin="17.7087" TopMargin="24.6292" BottomMargin="696.3708" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="17" Scale9Height="25" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="47.0000" Y="47.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="982.7913" Y="719.8708" />
            <Scale ScaleX="1.2000" ScaleY="1.2000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.9598" Y="0.9373" />
            <PreSize X="0.0459" Y="0.0612" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="Normal" Path="mjstory/popup/popup_ratebuy_btclose.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="rate_bg" ActionTag="521911233" Tag="105" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="212.0000" RightMargin="212.0000" TopMargin="-26.9000" BottomMargin="-30.1000" LeftEage="152" RightEage="152" TopEage="171" BottomEage="171" Scale9OriginX="152" Scale9OriginY="171" Scale9Width="296" Scale9Height="483" ctype="ImageViewObjectData">
            <Size X="600.0000" Y="825.0000" />
            <Children>
              <AbstractNodeData Name="layout_touch" ActionTag="659817662" Tag="34" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="600.0000" Y="825.0000" />
                <Children>
                  <AbstractNodeData Name="Image_5" ActionTag="-459463414" Tag="697" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="71.0000" RightMargin="71.0000" TopMargin="338.4734" BottomMargin="238.5266" LeftEage="93" RightEage="93" TopEage="81" BottomEage="81" Scale9OriginX="93" Scale9OriginY="81" Scale9Width="272" Scale9Height="86" ctype="ImageViewObjectData">
                    <Size X="458.0000" Y="248.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="300.0000" Y="362.5266" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.4394" />
                    <PreSize X="0.7633" Y="0.3006" />
                    <FileData Type="Normal" Path="mjstory/popup/popup_rate_heart.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition />
                <PreSize X="1.0000" Y="1.0000" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="num_storieslb" ActionTag="-2138273510" Tag="4" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="124.0600" RightMargin="75.9400" TopMargin="527.2498" BottomMargin="237.7502" IsCustomSize="True" FontSize="25" LabelText="Unlimited access in 2 days" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="400.0000" Y="60.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="324.0600" Y="267.7502" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="40" G="127" B="157" />
                <PrePosition X="0.5401" Y="0.3245" />
                <PreSize X="0.6667" Y="0.0727" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="title" ActionTag="-1905429183" Tag="5" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="98.0000" RightMargin="98.0000" TopMargin="265.4105" BottomMargin="510.5895" FontSize="40" LabelText="RATE US AND WIN!" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="404.0000" Y="49.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="300.0000" Y="535.0895" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="41" G="168" B="211" />
                <PrePosition X="0.5000" Y="0.6486" />
                <PreSize X="0.6733" Y="0.0594" />
                <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="content" ActionTag="100557992" Tag="6" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="50.0000" RightMargin="50.0000" TopMargin="609.4970" BottomMargin="125.5030" IsCustomSize="True" FontSize="25" LabelText="Hãy đánh giá Monkey Junior Stories&#xA; để đọc thêm truyện miễn phí" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="500.0000" Y="90.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="300.0000" Y="170.5030" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="40" G="127" B="157" />
                <PrePosition X="0.5000" Y="0.2067" />
                <PreSize X="0.8333" Y="0.1091" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btrate" ActionTag="1445712659" CallBackType="Click" CallBackName="onRate" Tag="7" IconVisible="False" LeftMargin="39.3696" RightMargin="36.6304" TopMargin="704.1691" BottomMargin="31.8309" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="494" Scale9Height="67" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="524.0000" Y="89.0000" />
                <Children>
                  <AbstractNodeData Name="title" ActionTag="-2029931772" Tag="8" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="220.5000" RightMargin="220.5000" TopMargin="23.0000" BottomMargin="23.0000" FontSize="35" LabelText="Rate" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="83.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="262.0000" Y="44.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.1584" Y="0.4831" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="301.3696" Y="76.3309" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5023" Y="0.0925" />
                <PreSize X="0.8733" Y="0.1079" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="mjstory/popup/ratebonus_bt.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/popup/ratebonus_bt.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="382.4000" />
            <Scale ScaleX="0.9000" ScaleY="0.9000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.4979" />
            <PreSize X="0.5859" Y="1.0742" />
            <FileData Type="Normal" Path="mjstory/popup/ratebonus_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>