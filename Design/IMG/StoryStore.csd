<GameFile>
  <PropertyGroup Name="StoryStore" Type="Scene" ID="a5148272-cf96-4f4d-b2ec-3f8c900be891" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" ActivedAnimationName="show" />
      <AnimationList>
        <AnimationInfo Name="show" StartIndex="0" EndIndex="50">
          <RenderColor A="255" R="173" G="216" B="230" />
        </AnimationInfo>
        <AnimationInfo Name="action" StartIndex="100" EndIndex="305">
          <RenderColor A="255" R="221" G="160" B="221" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Scene" CustomClassName="StoryStore" Tag="255" ctype="GameNodeObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="touchpanel" ActionTag="1071100505" CallBackType="Click" CallBackName="onTouchPanel" Tag="20" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
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
          <AbstractNodeData Name="bgpanel" ActionTag="-1087567578" Tag="17" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="title" ActionTag="-834244473" Tag="18" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="TopEdge" LeftMargin="-302.5000" RightMargin="-302.5000" TopMargin="-9.9994" BottomMargin="670.9994" LeftEage="537" RightEage="537" TopEage="35" BottomEage="35" Scale9OriginX="537" Scale9OriginY="35" Scale9Width="555" Scale9Height="37" ctype="ImageViewObjectData">
            <Size X="1629.0000" Y="107.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
            <Position X="512.0000" Y="777.9994" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="1.0130" />
            <PreSize X="1.5908" Y="0.1393" />
            <FileData Type="Normal" Path="hd/story/storystore_navi.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="btorder" ActionTag="-685743750" CallBackType="Click" CallBackName="onChoosOrder" Tag="19" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="30.0000" RightMargin="934.0000" TopMargin="13.3000" BottomMargin="692.7000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="30" Scale9Height="40" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="60.0000" Y="62.0000" />
            <AnchorPoint ScaleY="0.5000" />
            <Position X="30.0000" Y="723.7000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.0293" Y="0.9423" />
            <PreSize X="0.0586" Y="0.0807" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="PlistSubImage" Path="storystore/storystore_btorder.png" Plist="hd/story/storystore.plist" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="PlistSubImage" Path="storystore/storystore_btorder.png" Plist="hd/story/storystore.plist" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="btlang" ActionTag="1833937934" CallBackType="Click" CallBackName="onChooseLanguage" Tag="20" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="127.9426" RightMargin="645.0574" TopMargin="18.3003" BottomMargin="701.6997" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="221" Scale9Height="26" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="251.0000" Y="48.0000" />
            <Children>
              <AbstractNodeData Name="arrow" ActionTag="1696470485" Tag="22" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="225.0033" RightMargin="9.9967" TopMargin="19.0000" BottomMargin="19.0000" LeftEage="5" RightEage="5" TopEage="3" BottomEage="3" Scale9OriginX="5" Scale9OriginY="3" Scale9Width="6" Scale9Height="4" ctype="ImageViewObjectData">
                <Size X="16.0000" Y="10.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="233.0033" Y="24.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9283" Y="0.5000" />
                <PreSize X="0.0637" Y="0.2083" />
                <FileData Type="PlistSubImage" Path="storystore/storystore_arrow.png" Plist="hd/story/storystore.plist" />
              </AbstractNodeData>
              <AbstractNodeData Name="img_lang" ActionTag="-1092832132" Tag="23" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="6.8268" RightMargin="194.1732" TopMargin="-1.0000" BottomMargin="-1.0000" LeftEage="5" RightEage="5" TopEage="3" BottomEage="3" Scale9OriginX="5" Scale9OriginY="3" Scale9Width="40" Scale9Height="44" ctype="ImageViewObjectData">
                <Size X="50.0000" Y="50.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="31.8268" Y="24.0000" />
                <Scale ScaleX="0.8500" ScaleY="0.8500" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1268" Y="0.5000" />
                <PreSize X="0.1992" Y="1.0417" />
                <FileData Type="PlistSubImage" Path="storystore/storystore_flag.png" Plist="hd/story/storystore.plist" />
              </AbstractNodeData>
              <AbstractNodeData Name="label_lang" ActionTag="-1700997064" Tag="24" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="58.7591" RightMargin="64.2409" TopMargin="8.0000" BottomMargin="8.0000" FontSize="27" LabelText="English US" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="128.0000" Y="32.0000" />
                <AnchorPoint ScaleY="0.5000" />
                <Position X="58.7591" Y="24.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.2341" Y="0.5000" />
                <PreSize X="0.5100" Y="0.6667" />
                <FontResource Type="Normal" Path="fonts/Lato-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleY="0.5000" />
            <Position X="127.9426" Y="725.6997" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.1249" Y="0.9449" />
            <PreSize X="0.2451" Y="0.0625" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="PlistSubImage" Path="storystore/storystore_btLang.png" Plist="hd/story/storystore.plist" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="btmybook" ActionTag="-1607569099" CallBackType="Click" CallBackName="onChooseMybook" Tag="25" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="410.6094" RightMargin="362.3906" TopMargin="18.3000" BottomMargin="701.7000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="221" Scale9Height="26" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="251.0000" Y="48.0000" />
            <Children>
              <AbstractNodeData Name="arrow" ActionTag="-1992733637" Tag="26" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="225.0033" RightMargin="9.9967" TopMargin="19.0000" BottomMargin="19.0000" LeftEage="5" RightEage="5" TopEage="3" BottomEage="3" Scale9OriginX="5" Scale9OriginY="3" Scale9Width="6" Scale9Height="4" ctype="ImageViewObjectData">
                <Size X="16.0000" Y="10.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="233.0033" Y="24.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9283" Y="0.5000" />
                <PreSize X="0.0637" Y="0.2083" />
                <FileData Type="PlistSubImage" Path="storystore/storystore_arrow.png" Plist="hd/story/storystore.plist" />
              </AbstractNodeData>
              <AbstractNodeData Name="img_mybook" ActionTag="-2073477874" Tag="27" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="16.3268" RightMargin="203.6732" TopMargin="7.0000" BottomMargin="7.0000" LeftEage="5" RightEage="5" TopEage="3" BottomEage="3" Scale9OriginX="5" Scale9OriginY="3" Scale9Width="21" Scale9Height="28" ctype="ImageViewObjectData">
                <Size X="31.0000" Y="34.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="31.8268" Y="24.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1268" Y="0.5000" />
                <PreSize X="0.1235" Y="0.7083" />
                <FileData Type="PlistSubImage" Path="storystore/storystore_allbookicon.png" Plist="hd/story/storystore.plist" />
              </AbstractNodeData>
              <AbstractNodeData Name="label_mybook" ActionTag="-987041475" Tag="28" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="58.7591" RightMargin="82.2409" TopMargin="8.0000" BottomMargin="8.0000" FontSize="27" LabelText="All books" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="110.0000" Y="32.0000" />
                <AnchorPoint ScaleY="0.5000" />
                <Position X="58.7591" Y="24.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.2341" Y="0.5000" />
                <PreSize X="0.4382" Y="0.6667" />
                <FontResource Type="Normal" Path="fonts/Lato-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleY="0.5000" />
            <Position X="410.6094" Y="725.7000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.4010" Y="0.9449" />
            <PreSize X="0.2451" Y="0.0625" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="PlistSubImage" Path="storystore/storystore_btLang.png" Plist="hd/story/storystore.plist" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="btclose" ActionTag="408418135" CallBackType="Click" CallBackName="onClose" Tag="29" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="TopEdge" LeftMargin="956.9106" RightMargin="19.0894" TopMargin="19.3004" BottomMargin="698.6996" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="18" Scale9Height="28" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="48.0000" Y="50.0000" />
            <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
            <Position X="1004.9106" Y="723.6996" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.9814" Y="0.9423" />
            <PreSize X="0.0469" Y="0.0651" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="PlistSubImage" Path="storystore/storystore_btclose.png" Plist="hd/story/storystore.plist" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="img_order" ActionTag="654738309" Tag="30" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="27.5788" RightMargin="746.4212" TopMargin="79.9775" BottomMargin="471.0225" LeftEage="82" RightEage="82" TopEage="71" BottomEage="71" Scale9OriginX="82" Scale9OriginY="71" Scale9Width="86" Scale9Height="75" ctype="ImageViewObjectData">
            <Size X="250.0000" Y="217.0000" />
            <Children>
              <AbstractNodeData Name="bt1" ActionTag="2081194971" CallBackType="Click" CallBackName="onTouchOrder" Tag="10" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TopMargin="1.9096" BottomMargin="158.6704" TouchEnable="True" FontSize="20" ButtonText="MOST POPULAR" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="15" Scale9OriginY="6" Scale9Width="128" Scale9Height="12" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="250.0000" Y="56.4200" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="125.0000" Y="186.8804" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.8612" />
                <PreSize X="1.0000" Y="0.2600" />
                <FontResource Type="Normal" Path="fonts/Lato-Regular.ttf" Plist="" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="bt2" ActionTag="-1328180317" CallBackType="Click" CallBackName="onTouchOrder" Tag="11" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TopMargin="62.6262" BottomMargin="110.9738" TouchEnable="True" FontSize="20" ButtonText="NEWEST" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="15" Scale9OriginY="6" Scale9Width="56" Scale9Height="12" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="250.0000" Y="43.4000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="125.0000" Y="132.6738" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6114" />
                <PreSize X="1.0000" Y="0.2000" />
                <FontResource Type="Normal" Path="fonts/Lato-Regular.ttf" Plist="" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="bt3" ActionTag="-1340448966" CallBackType="Click" CallBackName="onTouchOrder" Tag="12" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TopMargin="110.7351" BottomMargin="62.8649" TouchEnable="True" FontSize="20" ButtonText="READING LEVEL" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="15" Scale9OriginY="6" Scale9Width="126" Scale9Height="12" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="250.0000" Y="43.4000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="125.0000" Y="84.5649" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.3897" />
                <PreSize X="1.0000" Y="0.2000" />
                <FontResource Type="Normal" Path="fonts/Lato-Regular.ttf" Plist="" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="bt4" ActionTag="-1474676197" CallBackType="Click" CallBackName="onTouchOrder" Tag="13" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TopMargin="157.5854" BottomMargin="2.9946" TouchEnable="True" FontSize="20" ButtonText="TITLE A-Z" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="15" Scale9OriginY="6" Scale9Width="63" Scale9Height="12" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="250.0000" Y="56.4200" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="125.0000" Y="31.2046" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1438" />
                <PreSize X="1.0000" Y="0.2600" />
                <FontResource Type="Normal" Path="fonts/Lato-Regular.ttf" Plist="" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleY="1.0000" />
            <Position X="27.5788" Y="688.0225" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.0269" Y="0.8959" />
            <PreSize X="0.2441" Y="0.2826" />
            <FileData Type="PlistSubImage" Path="storystore/storystore_orderbg.png" Plist="hd/story/storystore.plist" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>