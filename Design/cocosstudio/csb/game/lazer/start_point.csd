<GameFile>
  <PropertyGroup Name="start_point" Type="Layer" ID="d9ce6929-0c99-41ba-907e-3498ef2884b0" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="MJGameLazerFrameTop" Tag="22" ctype="GameLayerObjectData">
        <Size X="230.0000" Y="340.0000" />
        <Children>
          <AbstractNodeData Name="light_on" ActionTag="-814557801" VisibleForFrame="False" Tag="164" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="TopEdge" LeftMargin="58.5000" RightMargin="58.5000" TopMargin="-135.0000" BottomMargin="340.0000" FlipY="True" LeftEage="37" RightEage="37" TopEage="41" BottomEage="41" Scale9OriginX="37" Scale9OriginY="41" Scale9Width="39" Scale9Height="53" ctype="ImageViewObjectData">
            <Size X="113.0000" Y="135.0000" />
            <AnchorPoint ScaleX="0.5000" />
            <Position X="115.0000" Y="340.0000" />
            <Scale ScaleX="0.5000" ScaleY="0.5000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="1.0000" />
            <PreSize X="0.4913" Y="0.3971" />
            <FileData Type="Normal" Path="games/lazer/light_on.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="light_off" ActionTag="1795526193" Tag="165" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="TopEdge" LeftMargin="58.5000" RightMargin="58.5000" TopMargin="-135.0000" BottomMargin="340.0000" FlipY="True" LeftEage="37" RightEage="37" TopEage="41" BottomEage="41" Scale9OriginX="37" Scale9OriginY="41" Scale9Width="39" Scale9Height="53" ctype="ImageViewObjectData">
            <Size X="113.0000" Y="135.0000" />
            <AnchorPoint ScaleX="0.5000" />
            <Position X="115.0000" Y="340.0000" />
            <Scale ScaleX="0.5000" ScaleY="0.5000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="1.0000" />
            <PreSize X="0.4913" Y="0.3971" />
            <FileData Type="Normal" Path="games/lazer/light_off.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="Panel_1" ActionTag="-2127778929" Tag="110" IconVisible="False" LeftMargin="112.5000" RightMargin="112.5000" TopMargin="249.0000" BottomMargin="81.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="5.0000" Y="10.0000" />
            <AnchorPoint ScaleX="0.5000" />
            <Position X="115.0000" Y="81.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.2382" />
            <PreSize X="0.0217" Y="0.0294" />
            <SingleColor A="255" R="241" G="113" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="text_frame" ActionTag="-2108141002" CallBackType="Click" CallBackName="onBgFrame" Tag="166" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="TopEdge" TopMargin="67.5000" BottomMargin="87.5000" TouchEnable="True" Scale9Enable="True" LeftEage="61" RightEage="61" TopEage="41" BottomEage="41" Scale9OriginX="61" Scale9OriginY="41" Scale9Width="63" Scale9Height="43" ctype="ImageViewObjectData">
            <Size X="230.0000" Y="185.0000" />
            <Children>
              <AbstractNodeData Name="image" ActionTag="-785495429" Tag="265" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="25.0000" RightMargin="25.0000" TopMargin="32.5000" BottomMargin="32.5000" Scale9Width="46" Scale9Height="46" ctype="ImageViewObjectData">
                <Size X="180.0000" Y="120.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="115.0000" Y="92.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.7826" Y="0.6486" />
                <FileData Type="Default" Path="Default/ImageFile.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="text" ActionTag="-86382848" Tag="208" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="26.5000" RightMargin="26.5000" TopMargin="19.0000" BottomMargin="19.0000" FontSize="36" LabelText="poisonous&#xA;snake&#xA;whatever" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="177.0000" Y="147.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="115.0000" Y="92.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.7696" Y="0.7946" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="115.0000" Y="180.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5294" />
            <PreSize X="1.0000" Y="0.5441" />
            <FileData Type="Normal" Path="games/lazer/text_frame.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="plug" ActionTag="-1123281817" CallBackType="Touch" CallBackName="onTouchPlug" Tag="163" IconVisible="False" VerticalEdge="BottomEdge" LeftMargin="65.0000" RightMargin="65.0000" TopMargin="249.0001" BottomMargin="-9.0001" TouchEnable="True" LeftEage="14" RightEage="14" TopEage="19" BottomEage="19" Scale9OriginX="14" Scale9OriginY="19" Scale9Width="72" Scale9Height="62" ctype="ImageViewObjectData">
            <Size X="100.0000" Y="100.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="115.0000" Y="40.9999" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.1206" />
            <PreSize X="0.4348" Y="0.2941" />
            <FileData Type="Normal" Path="games/lazer/plug_out.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>