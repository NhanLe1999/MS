<GameFile>
  <PropertyGroup Name="StoryRateBonus" Type="Layer" ID="ce1039dd-c065-4adf-9e4c-75d9d7fef139" Version="3.10.0.0" />
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
        <Timeline ActionTag="521911233" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.0100" Y="0.0100">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="0.8500" Y="0.8500">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="0.8500" Y="0.8500">
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
          <AbstractNodeData Name="layout" ActionTag="974546587" Alpha="216" Tag="58" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FlipY="True" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="281" Scale9Height="66" ctype="ImageViewObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="-1.0000" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="mjstory/explore/mjsh_explore_bar.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="rate_bg" ActionTag="521911233" Tag="105" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="212.0000" RightMargin="212.0000" TopMargin="-33.9000" BottomMargin="-23.1000" LeftEage="152" RightEage="152" TopEage="171" BottomEage="171" Scale9OriginX="152" Scale9OriginY="171" Scale9Width="296" Scale9Height="483" ctype="ImageViewObjectData">
            <Size X="600.0000" Y="825.0000" />
            <Children>
              <AbstractNodeData Name="layout_touch" ActionTag="659817662" Tag="34" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="600.0000" Y="825.0000" />
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
              <AbstractNodeData Name="num_storieslb" ActionTag="-2138273510" VisibleForFrame="False" Tag="4" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="500.8400" RightMargin="55.1600" TopMargin="311.3658" BottomMargin="429.6342" FontSize="70" LabelText="4 " HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="44.0000" Y="84.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="522.8400" Y="471.6342" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="0" B="0" />
                <PrePosition X="0.8714" Y="0.5717" />
                <PreSize X="0.0733" Y="0.1018" />
                <FontResource Type="Normal" Path="fonts/Linotte Heavy.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="gif" ActionTag="-564351812" Tag="675" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="121.0000" RightMargin="121.0000" TopMargin="324.7847" BottomMargin="142.2153" LeftEage="118" RightEage="118" TopEage="118" BottomEage="118" Scale9OriginX="118" Scale9OriginY="118" Scale9Width="122" Scale9Height="122" ctype="ImageViewObjectData">
                <Size X="358.0000" Y="358.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="300.0000" Y="321.2153" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.3894" />
                <PreSize X="0.5967" Y="0.4339" />
                <FileData Type="Normal" Path="mjstory/popup/ratebonus_gift.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="numgift" ActionTag="782864338" Tag="674" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="78.0000" RightMargin="78.0000" TopMargin="397.4219" BottomMargin="389.5781" LeftEage="88" RightEage="88" TopEage="17" BottomEage="17" Scale9OriginX="88" Scale9OriginY="17" Scale9Width="268" Scale9Height="4" ctype="ImageViewObjectData">
                <Size X="444.0000" Y="38.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="300.0000" Y="408.5781" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4952" />
                <PreSize X="0.7400" Y="0.0461" />
                <FileData Type="Normal" Path="mjstory/popup/ratebonus_num_gift_1.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="alertlb" ActionTag="1241253127" Tag="132" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="163.0000" RightMargin="163.0000" TopMargin="321.3155" BottomMargin="466.6845" FontSize="30" LabelText="You have received" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="274.0000" Y="37.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="300.0000" Y="485.1845" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="40" G="127" B="157" />
                <PrePosition X="0.5000" Y="0.5881" />
                <PreSize X="0.4567" Y="0.0448" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="title" ActionTag="100557992" Tag="6" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="84.5000" RightMargin="84.5000" TopMargin="265.4789" BottomMargin="510.5211" FontSize="40" LabelText="CONGRATULATIONS!" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="431.0000" Y="49.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="300.0000" Y="535.0211" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="22" G="95" B="120" />
                <PrePosition X="0.5000" Y="0.6485" />
                <PreSize X="0.7183" Y="0.0594" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btok" ActionTag="1445712659" CallBackType="Click" CallBackName="onClose" Tag="7" IconVisible="False" LeftMargin="38.4897" RightMargin="37.5103" TopMargin="703.7947" BottomMargin="32.2053" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="494" Scale9Height="67" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="524.0000" Y="89.0000" />
                <Children>
                  <AbstractNodeData Name="title" ActionTag="-2029931772" Tag="8" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="230.5000" RightMargin="230.5000" TopMargin="25.3053" BottomMargin="9.6947" FontSize="45" LabelText="OK" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="63.0000" Y="54.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="262.0000" Y="36.6947" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.4123" />
                    <PreSize X="0.1202" Y="0.6067" />
                    <FontResource Type="Normal" Path="fonts/Linotte Heavy.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="300.4897" Y="76.7053" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5008" Y="0.0930" />
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
            <Position X="512.0000" Y="389.4000" />
            <Scale ScaleX="0.8500" ScaleY="0.8500" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5070" />
            <PreSize X="0.5859" Y="1.0742" />
            <FileData Type="Normal" Path="mjstory/popup/ratebonus_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>