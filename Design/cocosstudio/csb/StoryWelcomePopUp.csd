<GameFile>
  <PropertyGroup Name="StoryWelcomePopUp" Type="Layer" ID="2d3020e0-f933-4476-9d6d-aa6c82ed4b91" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="90" Speed="1.0000" ActivedAnimationName="show">
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
      </Animation>
      <AnimationList>
        <AnimationInfo Name="show" StartIndex="0" EndIndex="35">
          <RenderColor A="255" R="255" G="240" B="245" />
        </AnimationInfo>
        <AnimationInfo Name="hide" StartIndex="60" EndIndex="95">
          <RenderColor A="255" R="106" G="90" B="205" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="layer" CustomClassName="StoryWelcomePopUp" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="layout" ActionTag="-1628935586" Tag="167" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="204" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
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
          <AbstractNodeData Name="bg_ngang" ActionTag="521911233" Tag="105" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="99.5000" RightMargin="99.5000" TopMargin="23.0176" BottomMargin="44.9824" LeftEage="152" RightEage="152" TopEage="109" BottomEage="109" Scale9OriginX="152" Scale9OriginY="109" Scale9Width="521" Scale9Height="482" ctype="ImageViewObjectData">
            <Size X="825.0000" Y="700.0000" />
            <Children>
              <AbstractNodeData Name="title" ActionTag="-1905429183" Tag="5" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="294.0000" RightMargin="294.0000" TopMargin="253.2484" BottomMargin="407.7516" FontSize="32" LabelText="WELCOME TO" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="237.0000" Y="39.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="412.5000" Y="427.2516" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="40" G="127" B="157" />
                <PrePosition X="0.5000" Y="0.6104" />
                <PreSize X="0.2873" Y="0.0557" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_4" ActionTag="-493127865" Tag="71" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="306.0000" RightMargin="306.0000" TopMargin="309.5000" BottomMargin="219.5000" LeftEage="70" RightEage="70" TopEage="33" BottomEage="33" Scale9OriginX="70" Scale9OriginY="33" Scale9Width="73" Scale9Height="105" ctype="ImageViewObjectData">
                <Size X="213.0000" Y="171.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="412.5000" Y="305.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4357" />
                <PreSize X="0.2582" Y="0.2443" />
                <FileData Type="Normal" Path="mjstory/monkeystories_doc_new.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="content" ActionTag="100557992" Alpha="229" Tag="6" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="87.5000" RightMargin="87.5000" TopMargin="481.9962" BottomMargin="138.0038" IsCustomSize="True" FontSize="25" LabelText="Insprire a love of reading in your child." HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="650.0000" Y="80.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="412.5000" Y="178.0038" />
                <Scale ScaleX="0.9500" ScaleY="1.0000" />
                <CColor A="255" R="40" G="127" B="157" />
                <PrePosition X="0.5000" Y="0.2543" />
                <PreSize X="0.7879" Y="0.1143" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btsub" ActionTag="1445712659" CallBackType="Click" CallBackName="onSubscribe" Tag="7" IconVisible="False" LeftMargin="411.5000" RightMargin="40.5000" TopMargin="565.0000" BottomMargin="67.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="343" Scale9Height="46" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="373.0000" Y="68.0000" />
                <Children>
                  <AbstractNodeData Name="titlesub" ActionTag="-2029931772" Tag="8" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="99.5000" RightMargin="99.5000" TopMargin="20.5000" BottomMargin="20.5000" FontSize="22" LabelText="Subscribe Now" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="174.0000" Y="27.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="186.5000" Y="34.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.4665" Y="0.3971" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                    <OutlineColor A="255" R="77" G="77" B="77" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="598.0000" Y="101.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7248" Y="0.1443" />
                <PreSize X="0.4521" Y="0.0971" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="mjstory/popup/popup_welcome_bt2_ngang.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/popup/popup_welcome_bt2_ngang.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btfree" ActionTag="-193854663" CallBackType="Click" CallBackName="onFree" Tag="72" IconVisible="False" LeftMargin="43.0000" RightMargin="410.0000" TopMargin="564.5000" BottomMargin="66.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="342" Scale9Height="47" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="372.0000" Y="69.0000" />
                <Children>
                  <AbstractNodeData Name="titlefree" ActionTag="-259195062" Tag="73" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="31.5000" RightMargin="31.5000" TopMargin="21.0000" BottomMargin="21.0000" FontSize="22" LabelText="Try One Free Book Per Day" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="309.0000" Y="27.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="186.0000" Y="34.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.8306" Y="0.3913" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                    <OutlineColor A="255" R="77" G="77" B="77" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="229.0000" Y="101.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2776" Y="0.1443" />
                <PreSize X="0.4509" Y="0.0986" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="mjstory/popup/popup_welcome_bt1_ngang.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/popup/popup_welcome_bt1_ngang.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="394.9824" />
            <Scale ScaleX="0.0100" ScaleY="0.0100" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5143" />
            <PreSize X="0.8057" Y="0.9115" />
            <FileData Type="Normal" Path="mjstory/popup/popup_welcome_bg_ngang_new.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>