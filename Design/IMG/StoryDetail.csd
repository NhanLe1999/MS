<GameFile>
  <PropertyGroup Name="StoryDetail" Type="Layer" ID="c16dfccb-d0ac-4e89-9b09-c009c38c4f58" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" ActivedAnimationName="action" />
      <AnimationList>
        <AnimationInfo Name="action" StartIndex="0" EndIndex="80">
          <RenderColor A="255" R="221" G="160" B="221" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="StoryDetail" Tag="121" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="panel_bg" ActionTag="-290557222" Tag="46" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
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
          <AbstractNodeData Name="bg" ActionTag="1632543604" Tag="22" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-100.5000" RightMargin="-100.5000" TopMargin="-16.0000" BottomMargin="-16.0000" LeftEage="404" RightEage="404" TopEage="264" BottomEage="264" Scale9OriginX="404" Scale9OriginY="264" Scale9Width="417" Scale9Height="272" ctype="ImageViewObjectData">
            <Size X="1225.0000" Y="800.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.1963" Y="1.0417" />
            <FileData Type="Normal" Path="hd/story/storystore_downloadbg.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="btclose" ActionTag="-1503980240" CallBackType="Click" CallBackName="onClose" Tag="24" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="TopEdge" LeftMargin="950.5045" RightMargin="25.4955" TopMargin="25.2269" BottomMargin="692.7731" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="18" Scale9Height="28" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="48.0000" Y="50.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="974.5045" Y="717.7731" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.9517" Y="0.9346" />
            <PreSize X="0.0469" Y="0.0651" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="PlistSubImage" Path="storystore/storystore_btclose.png" Plist="hd/story/storystore.plist" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="img_book" ActionTag="-1127956774" Tag="23" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="32.5000" RightMargin="32.5000" TopMargin="100.4000" BottomMargin="23.6000" LeftEage="316" RightEage="316" TopEage="212" BottomEage="212" Scale9OriginX="316" Scale9OriginY="212" Scale9Width="327" Scale9Height="220" ctype="ImageViewObjectData">
            <Size X="959.0000" Y="644.0000" />
            <Children>
              <AbstractNodeData Name="panel_avatar" ActionTag="-1066790854" Tag="44" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="246.0134" RightMargin="662.9866" TopMargin="276.2632" BottomMargin="317.7368" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
                <Size X="50.0000" Y="50.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="271.0134" Y="342.7368" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2826" Y="0.5322" />
                <PreSize X="0.0521" Y="0.0776" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="panel_progress" ActionTag="332005339" VisibleForFrame="False" Tag="39" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="478.4925" RightMargin="80.5075" TopMargin="287.9200" BottomMargin="56.0800" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="400.0000" Y="300.0000" />
                <Children>
                  <AbstractNodeData Name="progressbg" ActionTag="-377994518" Tag="43" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="43.5000" RightMargin="43.5000" TopMargin="186.0000" BottomMargin="48.0000" LeftEage="103" RightEage="103" TopEage="21" BottomEage="21" Scale9OriginX="103" Scale9OriginY="21" Scale9Width="107" Scale9Height="24" ctype="ImageViewObjectData">
                    <Size X="313.0000" Y="66.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.0000" Y="81.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.2700" />
                    <PreSize X="0.7825" Y="0.2200" />
                    <FileData Type="PlistSubImage" Path="storystore/storystore_progressbg.png" Plist="hd/story/storystore.plist" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_status" ActionTag="-2016686706" Tag="41" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="113.5000" RightMargin="113.5000" TopMargin="126.1037" BottomMargin="137.8963" FontSize="30" LabelText="Please wati..." HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="173.0000" Y="36.0000" />
                    <AnchorPoint ScaleX="0.5000" />
                    <Position X="200.0000" Y="137.8963" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="108" G="146" B="244" />
                    <PrePosition X="0.5000" Y="0.4597" />
                    <PreSize X="0.4325" Y="0.1200" />
                    <FontResource Type="Normal" Path="fonts/Lato-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_percent" ActionTag="998357155" Tag="42" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="170.0000" RightMargin="170.0000" TopMargin="199.7411" BottomMargin="64.2589" FontSize="30" LabelText="70%" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="60.0000" Y="36.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.0000" Y="82.2589" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.2742" />
                    <PreSize X="0.1500" Y="0.1200" />
                    <FontResource Type="Normal" Path="fonts/Lato-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="678.4925" Y="206.0800" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7075" Y="0.3200" />
                <PreSize X="0.4171" Y="0.4658" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="panel_download" ActionTag="-597444152" VisibleForFrame="False" Tag="32" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="478.4925" RightMargin="80.5075" TopMargin="287.9200" BottomMargin="56.0800" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="400.0000" Y="300.0000" />
                <Children>
                  <AbstractNodeData Name="btdownload" ActionTag="1897983621" CallBackType="Click" CallBackName="onDownload" Tag="33" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="43.5000" RightMargin="43.5000" TopMargin="185.9996" BottomMargin="48.0004" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="283" Scale9Height="44" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="313.0000" Y="66.0000" />
                    <Children>
                      <AbstractNodeData Name="text_btdownload" ActionTag="-2057047211" Tag="38" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="24.7495" RightMargin="82.2505" TopMargin="15.0000" BottomMargin="15.0000" FontSize="30" LabelText="Download now" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="206.0000" Y="36.0000" />
                        <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                        <Position X="230.7495" Y="33.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="77" G="77" B="77" />
                        <PrePosition X="0.7372" Y="0.5000" />
                        <PreSize X="0.6581" Y="0.5455" />
                        <FontResource Type="Normal" Path="fonts/Lato-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.0000" Y="81.0004" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.2700" />
                    <PreSize X="0.7825" Y="0.2200" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="PlistSubImage" Path="storystore/storystore_btdownload.png" Plist="hd/story/storystore.plist" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_ttitledownload" ActionTag="10337274" Tag="37" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="36.5000" RightMargin="36.5000" TopMargin="102.1037" BottomMargin="137.8963" FontSize="25" LabelText="you must download this story&#xA; before reading it" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="327.0000" Y="60.0000" />
                    <AnchorPoint ScaleX="0.5000" />
                    <Position X="200.0000" Y="137.8963" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="108" G="146" B="244" />
                    <PrePosition X="0.5000" Y="0.4597" />
                    <PreSize X="0.8175" Y="0.2000" />
                    <FontResource Type="Normal" Path="fonts/Lato-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="678.4925" Y="206.0800" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7075" Y="0.3200" />
                <PreSize X="0.4171" Y="0.4658" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="panel_read" ActionTag="2081425533" VisibleForFrame="False" Tag="29" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="478.4925" RightMargin="80.5075" TopMargin="287.9200" BottomMargin="56.0800" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                <Size X="400.0000" Y="300.0000" />
                <Children>
                  <AbstractNodeData Name="btreadtome" ActionTag="1608171577" CallBackType="Click" CallBackName="onReadtome" Tag="27" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="53.5000" RightMargin="53.5000" TopMargin="36.0000" BottomMargin="198.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="263" Scale9Height="44" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="293.0000" Y="66.0000" />
                    <Children>
                      <AbstractNodeData Name="text_readtome" ActionTag="369153193" Tag="48" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="86.4350" RightMargin="69.5650" TopMargin="15.0000" BottomMargin="19.0000" FontSize="27" LabelText="Read to me" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="137.0000" Y="32.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="86.4350" Y="35.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="77" G="77" B="77" />
                        <PrePosition X="0.2950" Y="0.5303" />
                        <PreSize X="0.4676" Y="0.4848" />
                        <FontResource Type="Normal" Path="fonts/Lato-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.0000" Y="231.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.7700" />
                    <PreSize X="0.7325" Y="0.2200" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="PlistSubImage" Path="storystore/storystore_btreadtome.png" Plist="hd/story/storystore.plist" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btautoplay" ActionTag="-58167233" CallBackType="Click" CallBackName="onAutoplay" Tag="30" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="53.5000" RightMargin="53.5000" TopMargin="117.0000" BottomMargin="117.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="263" Scale9Height="44" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="293.0000" Y="66.0000" />
                    <Children>
                      <AbstractNodeData Name="text_autoplay" ActionTag="901000450" Tag="49" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="86.4350" RightMargin="69.5650" TopMargin="17.0000" BottomMargin="17.0000" FontSize="27" LabelText="Read to me" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="137.0000" Y="32.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="86.4350" Y="33.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="77" G="77" B="77" />
                        <PrePosition X="0.2950" Y="0.5000" />
                        <PreSize X="0.4676" Y="0.4848" />
                        <FontResource Type="Normal" Path="fonts/Lato-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.0000" Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.7325" Y="0.2200" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="PlistSubImage" Path="storystore/storystore_btautoplay.png" Plist="hd/story/storystore.plist" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btreaditmyself" ActionTag="-705836569" CallBackType="Click" CallBackName="onReaditmyself" Tag="31" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="53.5000" RightMargin="53.5000" TopMargin="198.0000" BottomMargin="36.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="263" Scale9Height="44" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="293.0000" Y="66.0000" />
                    <Children>
                      <AbstractNodeData Name="text_readitmyseft" ActionTag="454238528" Tag="50" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="86.4350" RightMargin="33.5650" TopMargin="17.0000" BottomMargin="17.0000" FontSize="27" LabelText="Read it myseft" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="173.0000" Y="32.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="86.4350" Y="33.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="77" G="77" B="77" />
                        <PrePosition X="0.2950" Y="0.5000" />
                        <PreSize X="0.5904" Y="0.4848" />
                        <FontResource Type="Normal" Path="fonts/Lato-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.0000" Y="69.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.2300" />
                    <PreSize X="0.7325" Y="0.2200" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="PlistSubImage" Path="storystore/storystore_btreaditmyself.png" Plist="hd/story/storystore.plist" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="678.4925" Y="206.0800" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7075" Y="0.3200" />
                <PreSize X="0.4171" Y="0.4658" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="345.6000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.4500" />
            <PreSize X="0.9365" Y="0.8385" />
            <FileData Type="PlistSubImage" Path="storystore/storystore_bookbg.png" Plist="hd/story/storystore.plist" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>