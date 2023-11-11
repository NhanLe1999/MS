<GameFile>
  <PropertyGroup Name="ProfileAgePopup" Type="Layer" ID="034a1b52-65d0-42e2-8a04-8e22b5021db8" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="90" Speed="1.0000" ActivedAnimationName="hide">
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
      <ObjectData Name="layer" CustomClassName="APProfileAgeView" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="layout" ActionTag="-1628935586" CallBackType="Click" CallBackName="onClose" Tag="167" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="204" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
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
          <AbstractNodeData Name="Panel_1" ActionTag="521911233" Tag="105" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="100.1472" RightMargin="104.6528" TopMargin="76.8000" BottomMargin="76.8000" TouchEnable="True" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="45" Scale9Height="45" ctype="ImageViewObjectData">
            <Size X="819.2000" Y="614.4000" />
            <Children>
              <AbstractNodeData Name="age_2" ActionTag="863375097" CallBackType="Click" CallBackName="onSelectAge" Tag="2" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="39.9770" RightMargin="619.2230" TopMargin="202.4000" BottomMargin="330.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ColorAngle="90.0000" Scale9Enable="True" LeftEage="117" RightEage="117" TopEage="60" BottomEage="60" Scale9OriginX="117" Scale9OriginY="60" Scale9Width="122" Scale9Height="62" ctype="PanelObjectData">
                <Size X="160.0000" Y="82.0000" />
                <Children>
                  <AbstractNodeData Name="Image_2_0" ActionTag="1107244022" VisibleForFrame="False" Tag="228" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="52.5000" RightMargin="92.5000" TopMargin="23.5000" BottomMargin="23.5000" Scale9Enable="True" LeftEage="1" RightEage="1" TopEage="1" BottomEage="1" Scale9OriginX="1" Scale9OriginY="1" Scale9Width="57" Scale9Height="123" ctype="ImageViewObjectData">
                    <Size X="15.0000" Y="35.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="60.0000" Y="41.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3750" Y="0.5000" />
                    <PreSize X="0.0938" Y="0.4268" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/icon_back.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Text_1_0_0" ActionTag="1382667758" Tag="98" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="54.5000" RightMargin="54.5000" TopMargin="14.0000" BottomMargin="14.0000" FontSize="44" LabelText="&lt;3" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="51.0000" Y="54.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="80.0000" Y="41.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="41" G="176" B="223" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.3187" Y="0.6585" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="39.9770" Y="330.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0488" Y="0.5371" />
                <PreSize X="0.1953" Y="0.1335" />
                <FileData Type="Normal" Path="mjstory/new_ui/login/button age 2.png" Plist="" />
                <SingleColor A="255" R="38" G="176" B="230" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="age_3" ActionTag="-1830703667" CallBackType="Click" CallBackName="onSelectAge" Tag="3" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="230.0314" RightMargin="429.1686" TopMargin="202.4000" BottomMargin="330.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Enable="True" LeftEage="117" RightEage="117" TopEage="60" BottomEage="60" Scale9OriginX="117" Scale9OriginY="60" Scale9Width="122" Scale9Height="62" ctype="PanelObjectData">
                <Size X="160.0000" Y="82.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1_0_0" ActionTag="1790412613" Tag="100" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="67.0000" RightMargin="67.0000" TopMargin="14.0000" BottomMargin="14.0000" FontSize="44" LabelText="3" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="26.0000" Y="54.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="80.0000" Y="41.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="41" G="176" B="223" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.1625" Y="0.6585" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="230.0314" Y="330.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2808" Y="0.5371" />
                <PreSize X="0.1953" Y="0.1335" />
                <FileData Type="Normal" Path="mjstory/new_ui/login/button age 2.png" Plist="" />
                <SingleColor A="255" R="38" G="176" B="230" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="age_4" ActionTag="-921760316" CallBackType="Click" CallBackName="onSelectAge" Tag="4" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="420.0039" RightMargin="239.1961" TopMargin="202.4000" BottomMargin="330.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Enable="True" LeftEage="117" RightEage="117" TopEage="60" BottomEage="60" Scale9OriginX="117" Scale9OriginY="60" Scale9Width="122" Scale9Height="62" ctype="PanelObjectData">
                <Size X="160.0000" Y="82.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1_0_0" ActionTag="930419146" Tag="102" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="64.0000" RightMargin="64.0000" TopMargin="14.0000" BottomMargin="14.0000" FontSize="44" LabelText="4" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="32.0000" Y="54.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="80.0000" Y="41.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="41" G="176" B="223" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.2000" Y="0.6585" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="420.0039" Y="330.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5127" Y="0.5371" />
                <PreSize X="0.1953" Y="0.1335" />
                <FileData Type="Normal" Path="mjstory/new_ui/login/button age 2.png" Plist="" />
                <SingleColor A="255" R="38" G="176" B="230" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="age_5" ActionTag="-435264114" CallBackType="Click" CallBackName="onSelectAge" Tag="5" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="609.9763" RightMargin="49.2237" TopMargin="202.4000" BottomMargin="330.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Enable="True" LeftEage="117" RightEage="117" TopEage="60" BottomEage="60" Scale9OriginX="117" Scale9OriginY="60" Scale9Width="122" Scale9Height="62" ctype="PanelObjectData">
                <Size X="160.0000" Y="82.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1_0_0" ActionTag="365042718" Tag="104" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="66.5000" RightMargin="66.5000" TopMargin="14.0000" BottomMargin="14.0000" FontSize="44" LabelText="5" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="27.0000" Y="54.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="80.0000" Y="41.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="41" G="176" B="223" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.1688" Y="0.6585" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="609.9763" Y="330.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7446" Y="0.5371" />
                <PreSize X="0.1953" Y="0.1335" />
                <FileData Type="Normal" Path="mjstory/new_ui/login/button age 2.png" Plist="" />
                <SingleColor A="255" R="38" G="176" B="230" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="age_6" ActionTag="-1658774814" CallBackType="Click" CallBackName="onSelectAge" Tag="6" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="39.9770" RightMargin="619.2230" TopMargin="332.4000" BottomMargin="200.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Enable="True" LeftEage="117" RightEage="117" TopEage="60" BottomEage="60" Scale9OriginX="117" Scale9OriginY="60" Scale9Width="122" Scale9Height="62" ctype="PanelObjectData">
                <Size X="160.0000" Y="82.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1_0_0" ActionTag="57171149" Tag="106" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="65.5000" RightMargin="65.5000" TopMargin="14.0000" BottomMargin="14.0000" FontSize="44" LabelText="6" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="29.0000" Y="54.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="80.0000" Y="41.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="41" G="176" B="223" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.1813" Y="0.6585" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="39.9770" Y="200.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0488" Y="0.3255" />
                <PreSize X="0.1953" Y="0.1335" />
                <FileData Type="Normal" Path="mjstory/new_ui/login/button age 2.png" Plist="" />
                <SingleColor A="255" R="38" G="176" B="230" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="age_7" ActionTag="364423540" CallBackType="Click" CallBackName="onSelectAge" Tag="7" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="230.0314" RightMargin="429.1686" TopMargin="332.4000" BottomMargin="200.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Enable="True" LeftEage="117" RightEage="117" TopEage="60" BottomEage="60" Scale9OriginX="117" Scale9OriginY="60" Scale9Width="122" Scale9Height="62" ctype="PanelObjectData">
                <Size X="160.0000" Y="82.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1_0_0" ActionTag="1331413615" Tag="108" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="66.0000" RightMargin="66.0000" TopMargin="14.0000" BottomMargin="14.0000" FontSize="44" LabelText="7" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="28.0000" Y="54.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="80.0000" Y="41.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="41" G="176" B="223" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.1750" Y="0.6585" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="230.0314" Y="200.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2808" Y="0.3255" />
                <PreSize X="0.1953" Y="0.1335" />
                <FileData Type="Normal" Path="mjstory/new_ui/login/button age 2.png" Plist="" />
                <SingleColor A="255" R="38" G="176" B="230" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="age_8" ActionTag="-431506534" CallBackType="Click" CallBackName="onSelectAge" Tag="8" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="420.0039" RightMargin="239.1961" TopMargin="332.4000" BottomMargin="200.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Enable="True" LeftEage="117" RightEage="117" TopEage="60" BottomEage="60" Scale9OriginX="117" Scale9OriginY="60" Scale9Width="122" Scale9Height="62" ctype="PanelObjectData">
                <Size X="160.0000" Y="82.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1_0_0" ActionTag="-1240845241" Tag="110" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="65.5000" RightMargin="65.5000" TopMargin="14.0000" BottomMargin="14.0000" FontSize="44" LabelText="8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="29.0000" Y="54.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="80.0000" Y="41.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="41" G="176" B="223" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.1813" Y="0.6585" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="420.0039" Y="200.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5127" Y="0.3255" />
                <PreSize X="0.1953" Y="0.1335" />
                <FileData Type="Normal" Path="mjstory/new_ui/login/button age 2.png" Plist="" />
                <SingleColor A="255" R="38" G="176" B="230" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="age_9" ActionTag="1182902499" CallBackType="Click" CallBackName="onSelectAge" Tag="9" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="609.9763" RightMargin="49.2237" TopMargin="332.4000" BottomMargin="200.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Enable="True" LeftEage="117" RightEage="117" TopEage="60" BottomEage="60" Scale9OriginX="117" Scale9OriginY="60" Scale9Width="122" Scale9Height="62" ctype="PanelObjectData">
                <Size X="160.0000" Y="82.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1_0_0" ActionTag="-1218684878" Tag="112" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="53.0000" RightMargin="53.0000" TopMargin="14.0000" BottomMargin="14.0000" FontSize="44" LabelText="&gt;8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="54.0000" Y="54.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="80.0000" Y="41.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="41" G="176" B="223" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.3375" Y="0.6585" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_2_0" ActionTag="352649673" VisibleForFrame="False" Tag="229" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="51.0000" RightMargin="91.0000" TopMargin="23.5000" BottomMargin="23.5000" FlipX="True" Scale9Enable="True" LeftEage="1" RightEage="1" TopEage="1" BottomEage="1" Scale9OriginX="1" Scale9OriginY="1" Scale9Width="57" Scale9Height="123" ctype="ImageViewObjectData">
                    <Size X="18.0000" Y="35.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="60.0000" Y="41.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3750" Y="0.5000" />
                    <PreSize X="0.1125" Y="0.4268" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/icon_back.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="609.9763" Y="200.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7446" Y="0.3255" />
                <PreSize X="0.1953" Y="0.1335" />
                <FileData Type="Normal" Path="mjstory/new_ui/login/button age 2.png" Plist="" />
                <SingleColor A="255" R="38" G="176" B="230" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="header" ActionTag="485827049" Tag="113" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" BottomMargin="534.4000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="819.2000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1" ActionTag="346659697" Tag="114" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="174.6000" RightMargin="174.6000" TopMargin="15.5000" BottomMargin="15.5000" FontSize="40" LabelText="Con bạn bao nhiêu tuổi" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="470.0000" Y="49.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="409.6000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.5737" Y="0.6125" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btn_close" ActionTag="-1640209626" CallBackType="Click" CallBackName="onClose" Tag="115" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="748.2400" RightMargin="10.9600" TopMargin="10.0000" BottomMargin="10.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="30" Scale9Height="38" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="60.0000" Y="60.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="778.2400" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9500" Y="0.5000" />
                    <PreSize X="0.0732" Y="0.7500" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/button_sidebar_close.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="1.0000" />
                <Position Y="614.4000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition Y="1.0000" />
                <PreSize X="1.0000" Y="0.1302" />
                <SingleColor A="255" R="41" G="176" B="223" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_next" ActionTag="1981271285" CallBackType="Click" CallBackName="onNext" Tag="590" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="309.6000" RightMargin="309.6000" TopMargin="509.4000" BottomMargin="45.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="22" RightEage="22" TopEage="11" BottomEage="11" Scale9OriginX="22" Scale9OriginY="11" Scale9Width="113" Scale9Height="24" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="200.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1_0_0" ActionTag="-157180818" Tag="485" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="64.0000" RightMargin="64.0000" TopMargin="10.5000" BottomMargin="10.5000" FontSize="32" LabelText="Tiếp" HorizontalAlignmentType="HT_Right" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="72.0000" Y="39.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="100.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.3600" Y="0.6500" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_2" ActionTag="1722936776" VisibleForFrame="False" Tag="484" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="117.5000" RightMargin="67.5000" TopMargin="12.5000" BottomMargin="12.5000" FlipX="True" Scale9Enable="True" LeftEage="1" RightEage="1" TopEage="1" BottomEage="1" Scale9OriginX="1" Scale9OriginY="1" Scale9Width="57" Scale9Height="123" ctype="ImageViewObjectData">
                    <Size X="15.0000" Y="35.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="125.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.6250" Y="0.5000" />
                    <PreSize X="0.0750" Y="0.5833" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/icon_back.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="409.6000" Y="75.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1221" />
                <PreSize X="0.2441" Y="0.0977" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="509.7472" Y="384.0000" />
            <Scale ScaleX="0.0100" ScaleY="0.0100" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.4978" Y="0.5000" />
            <PreSize X="0.8000" Y="0.8000" />
            <FileData Type="Normal" Path="mjstory/dropdown/mj_level_background.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>