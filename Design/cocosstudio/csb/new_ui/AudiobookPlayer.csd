<GameFile>
  <PropertyGroup Name="AudiobookPlayer" Type="Layer" ID="957fb255-cb7d-4bd3-82ce-53bd4a0d9a10" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="MSAudiobookPlayer" Tag="116" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="controller" ActionTag="1873053235" Tag="31" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="Image_2" ActionTag="-609829275" Tag="30" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftEage="462" RightEage="462" TopEage="253" BottomEage="253" Scale9OriginX="462" Scale9OriginY="253" Scale9Width="476" Scale9Height="262" ctype="ImageViewObjectData">
                <Size X="1024.0000" Y="768.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="1.0000" Y="1.0000" />
                <FileData Type="Normal" Path="mjstory/new_ui/image_audiobook_player_background.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_back" ActionTag="-1530369332" CallBackType="Click" CallBackName="onBack" Tag="173" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="TopEdge" LeftMargin="931.5000" RightMargin="17.5000" TopMargin="11.5000" BottomMargin="681.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="53" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="75.0000" Y="75.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="969.0000" Y="719.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9463" Y="0.9362" />
                <PreSize X="0.0732" Y="0.0977" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_close.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="text_title" ActionTag="1247549377" Tag="33" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="75.0000" RightMargin="490.0000" TopMargin="24.5000" BottomMargin="694.5000" FontSize="40" LabelText="Hoàng tử chim hoàng " ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="459.0000" Y="49.0000" />
                <AnchorPoint ScaleY="0.5000" />
                <Position X="75.0000" Y="719.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0732" Y="0.9362" />
                <PreSize X="0.4482" Y="0.0638" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_play_main" ActionTag="-195211224" CallBackType="Click" CallBackName="onPlay" Tag="118" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="381.5000" RightMargin="381.5000" TopMargin="516.5000" BottomMargin="-9.5000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="231" Scale9Height="239" DisplayState="False" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="261.0000" Y="261.0000" />
                <Children>
                  <AbstractNodeData Name="download_progress" ActionTag="-1608564026" Tag="88" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="36.5000" RightMargin="36.5000" TopMargin="36.5000" BottomMargin="36.5000" ProgressInfo="0" ctype="LoadingBarObjectData">
                    <Size X="188.0000" Y="188.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="130.5000" Y="130.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.7203" Y="0.7203" />
                    <ImageFileData Type="Normal" Path="mjstory/new_ui/button_cover_progress.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="download_label" ActionTag="2020130960" Tag="40" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="6.5000" RightMargin="6.5000" TopMargin="111.0000" BottomMargin="111.0000" FontSize="30" LabelText="Downloading..." OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="248.0000" Y="39.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="130.5000" Y="130.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.9502" Y="0.1494" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Black.ttf" Plist="" />
                    <OutlineColor A="255" R="60" G="135" B="37" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="121.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1576" />
                <PreSize X="0.2549" Y="0.3398" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Normal" Path="mjstory/new_ui/button_cover_progress_background.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_cover_progress_background.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btdownload" ActionTag="-907910943" CallBackType="Click" CallBackName="onDownload" Tag="94" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="381.5000" RightMargin="381.5000" TopMargin="516.5000" BottomMargin="-9.5000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="231" Scale9Height="239" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="261.0000" Y="261.0000" />
                <Children>
                  <AbstractNodeData Name="text_btdownload" ActionTag="-712367752" Tag="95" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="68.5000" RightMargin="68.5000" TopMargin="142.1000" BottomMargin="89.9000" FontSize="22" LabelText="Get it now" HorizontalAlignmentType="HT_Center" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="124.0000" Y="29.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="130.5000" Y="104.4000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.4000" />
                    <PreSize X="0.4751" Y="0.1111" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                    <OutlineColor A="255" R="60" G="135" B="37" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_filesize" ActionTag="-1416760745" Tag="258" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="97.0000" RightMargin="97.0000" TopMargin="178.5300" BottomMargin="58.4700" FontSize="18" LabelText="1.33Mb" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="67.0000" Y="24.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="130.5000" Y="70.4700" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.2700" />
                    <PreSize X="0.2567" Y="0.0920" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                    <OutlineColor A="255" R="60" G="135" B="37" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="121.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1576" />
                <PreSize X="0.2549" Y="0.3398" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="mjstory/new_ui/button_cover_download_pressed.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_cover_download.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_retry" ActionTag="252906967" VisibleForFrame="False" CallBackType="Click" CallBackName="onDownload" Tag="96" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="381.5000" RightMargin="381.5000" TopMargin="516.5000" BottomMargin="-9.5000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="231" Scale9Height="239" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="261.0000" Y="261.0000" />
                <Children>
                  <AbstractNodeData Name="text_btdownload" ActionTag="989444314" Tag="97" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="68.5000" RightMargin="68.5000" TopMargin="142.1000" BottomMargin="89.9000" FontSize="22" LabelText="Get it now" HorizontalAlignmentType="HT_Center" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="124.0000" Y="29.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="130.5000" Y="104.4000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.4000" />
                    <PreSize X="0.4751" Y="0.1111" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                    <OutlineColor A="255" R="60" G="135" B="37" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="121.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1576" />
                <PreSize X="0.2549" Y="0.3398" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="mjstory/new_ui/button_cover_download_pressed.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_cover_download.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="content" ActionTag="1021941894" Tag="32" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="TopEdge" LeftMargin="60.9760" RightMargin="63.0240" TopMargin="126.0002" BottomMargin="241.9998" Scale9Enable="True" LeftEage="60" RightEage="60" TopEage="40" BottomEage="40" Scale9OriginX="60" Scale9OriginY="40" Scale9Width="63" Scale9Height="44" ctype="ImageViewObjectData">
                <Size X="900.0000" Y="400.0000" />
                <Children>
                  <AbstractNodeData Name="Panel_10" ActionTag="535485510" VisibleForFrame="False" Tag="140" IconVisible="False" HorizontalEdge="RightEdge" LeftMargin="510.0000" RightMargin="40.0000" TopMargin="289.5000" BottomMargin="109.5000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="350.0000" Y="1.0000" />
                    <AnchorPoint ScaleX="0.7000" ScaleY="0.5000" />
                    <Position X="755.0000" Y="110.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8389" Y="0.2750" />
                    <PreSize X="0.3889" Y="0.0025" />
                    <SingleColor A="255" R="0" G="0" B="0" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="list_extra" ActionTag="637929163" Tag="132" IconVisible="False" LeftMargin="310.0000" RightMargin="40.0000" TopMargin="295.0000" BottomMargin="45.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="0" ItemMargin="15" VerticalType="Align_VerticalCenter" ctype="ListViewObjectData">
                    <Size X="550.0000" Y="60.0000" />
                    <Children>
                      <AbstractNodeData Name="pextra_duration" ActionTag="-2103424857" Tag="133" IconVisible="False" RightMargin="440.0000" TopMargin="5.0000" BottomMargin="5.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="110.0000" Y="50.0000" />
                        <Children>
                          <AbstractNodeData Name="extra_duration" ActionTag="-1637154868" Tag="127" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="BottomEdge" RightMargin="49.0000" TopMargin="26.0000" FontSize="20" LabelText="5 min" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="61.0000" Y="24.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position Y="12.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition Y="0.2400" />
                            <PreSize X="0.5545" Y="0.4800" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                          <AbstractNodeData Name="extra_duration_title" ActionTag="628814916" Alpha="153" Tag="128" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" RightMargin="9.0000" BottomMargin="28.0000" FontSize="18" LabelText="Thời lượng:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="101.0000" Y="22.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position Y="39.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition Y="0.7800" />
                            <PreSize X="0.9182" Y="0.4400" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint />
                        <Position Y="5.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition Y="0.0833" />
                        <PreSize X="0.2000" Y="0.8333" />
                        <SingleColor A="255" R="150" G="200" B="255" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="pextra_grade" ActionTag="-199858420" ZOrder="1" Tag="134" IconVisible="False" LeftMargin="125.0000" RightMargin="275.0000" TopMargin="5.0000" BottomMargin="5.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="150.0000" Y="50.0000" />
                        <Children>
                          <AbstractNodeData Name="extra_level" ActionTag="-85132116" Tag="135" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="BottomEdge" RightMargin="89.0000" TopMargin="26.0000" FontSize="20" LabelText="5 min" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="61.0000" Y="24.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position Y="12.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition Y="0.2400" />
                            <PreSize X="0.4067" Y="0.4800" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                          <AbstractNodeData Name="extra_level_title" ActionTag="1381791921" Alpha="153" Tag="136" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" RightMargin="49.0000" BottomMargin="28.0000" FontSize="18" LabelText="Thời lượng:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="101.0000" Y="22.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position Y="39.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition Y="0.7800" />
                            <PreSize X="0.6733" Y="0.4400" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint />
                        <Position X="125.0000" Y="5.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.2273" Y="0.0833" />
                        <PreSize X="0.2727" Y="0.8333" />
                        <SingleColor A="255" R="150" G="200" B="255" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="pextra_extra" ActionTag="-1130361117" ZOrder="2" Tag="137" IconVisible="False" LeftMargin="290.0000" RightMargin="60.0000" TopMargin="5.0000" BottomMargin="5.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="200.0000" Y="50.0000" />
                        <Children>
                          <AbstractNodeData Name="extra" ActionTag="670948464" Tag="138" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="BottomEdge" RightMargin="139.0000" TopMargin="26.0000" FontSize="20" LabelText="5 min" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="61.0000" Y="24.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position Y="12.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition Y="0.2400" />
                            <PreSize X="0.3050" Y="0.4800" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                          <AbstractNodeData Name="extra_title" ActionTag="1159748888" Alpha="153" Tag="139" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" RightMargin="99.0000" BottomMargin="28.0000" FontSize="18" LabelText="Thời lượng:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="101.0000" Y="22.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position Y="39.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition Y="0.7800" />
                            <PreSize X="0.5050" Y="0.4400" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint />
                        <Position X="290.0000" Y="5.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5273" Y="0.0833" />
                        <PreSize X="0.3636" Y="0.8333" />
                        <SingleColor A="255" R="150" G="200" B="255" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="1.0000" />
                    <Position X="860.0000" Y="45.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9556" Y="0.1125" />
                    <PreSize X="0.6111" Y="0.1500" />
                    <SingleColor A="255" R="150" G="150" B="255" />
                    <FirstColor A="255" R="150" G="150" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="510.9760" Y="641.9998" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4990" Y="0.8359" />
                <PreSize X="0.8789" Y="0.5208" />
                <FileData Type="Normal" Path="mjstory/new_ui/frame_audiobook_content.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="story_container" Visible="False" ActionTag="-1001822706" Tag="220" IconVisible="False" PositionPercentXEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" VerticalEdge="TopEdge" TopMargin="125.0000" BottomMargin="102.0000" ClipAble="True" BackColorAlpha="165" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="1024.0000" Y="541.0000" />
                <Children>
                  <AbstractNodeData Name="text_story" Visible="False" ActionTag="193708865" Tag="22" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="TopEdge" LeftMargin="100.2505" RightMargin="99.7495" TopMargin="20.0000" BottomMargin="-477.0000" StretchWidthEnable="True" IsCustomSize="True" FontSize="50" LabelText="" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="824.0000" Y="998.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                    <Position X="512.2505" Y="521.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5002" Y="0.9630" />
                    <PreSize X="0.8047" Y="1.8447" />
                    <FontResource Type="Normal" Path="fonts/Vollkorn-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="512.0000" Y="643.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.8372" />
                <PreSize X="1.0000" Y="0.7044" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
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
          <AbstractNodeData Name="player" ActionTag="1596348044" Tag="210" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TopMargin="769.2288" BottomMargin="-101.2288" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="100.0000" />
            <Children>
              <AbstractNodeData Name="button_favourite" ActionTag="-1389730698" CallBackType="Click" CallBackName="onFavourite" Tag="122" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="RightEdge" LeftMargin="924.0004" RightMargin="19.9996" TopMargin="15.0000" BottomMargin="5.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="-15" Scale9OriginY="-6" Scale9Width="30" Scale9Height="12" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="80.0000" Y="80.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="964.0004" Y="45.0000" />
                <Scale ScaleX="1.2500" ScaleY="1.2500" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9414" Y="0.4500" />
                <PreSize X="0.0781" Y="0.8000" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_play" ActionTag="598479358" CallBackType="Click" CallBackName="onPlayerPlay" Tag="211" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="95.0000" RightMargin="839.0000" TopMargin="5.0000" BottomMargin="5.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="231" Scale9Height="239" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="90.0000" Y="90.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="140.0000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1367" Y="0.5000" />
                <PreSize X="0.0879" Y="0.9000" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_audiobook_play_control.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_pause" ActionTag="-1530603492" VisibleForFrame="False" CallBackType="Click" CallBackName="onPlayerPause" Tag="217" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="95.0000" RightMargin="839.0000" TopMargin="5.0000" BottomMargin="5.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="48" Scale9Height="56" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="90.0000" Y="90.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="140.0000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1367" Y="0.5000" />
                <PreSize X="0.0879" Y="0.9000" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_audiobook_pause.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_backward" ActionTag="246432355" CallBackType="Click" CallBackName="onPlayerBackward" Tag="119" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="28.5000" RightMargin="942.5000" TopMargin="23.5000" BottomMargin="23.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="23" Scale9Height="31" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="53.0000" Y="53.0000" />
                <Children>
                  <AbstractNodeData Name="layout_touch1" ActionTag="-1437033690" VisibleForFrame="False" Tag="34" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="53.0000" Y="53.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="26.5000" Y="26.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="55.0000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0537" Y="0.5000" />
                <PreSize X="0.0518" Y="0.5300" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_audiobook_backward.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_forward" ActionTag="-807854096" CallBackType="Click" CallBackName="onPlayerForward" Tag="120" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="198.5000" RightMargin="772.5000" TopMargin="23.5000" BottomMargin="23.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="23" Scale9Height="31" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="53.0000" Y="53.0000" />
                <Children>
                  <AbstractNodeData Name="layout_touch2" ActionTag="2029384564" VisibleForFrame="False" Tag="35" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="53.0000" Y="53.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="26.5000" Y="26.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="225.0000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2197" Y="0.5000" />
                <PreSize X="0.0518" Y="0.5300" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_audiobook_forward.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="progress" ActionTag="-67984387" Tag="117" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="RightEdge" LeftMargin="274.0000" RightMargin="150.0000" TopMargin="51.5000" BottomMargin="25.5000" TouchEnable="True" PercentInfo="100" ctype="SliderObjectData">
                <Size X="600.0000" Y="23.0000" />
                <Children>
                  <AbstractNodeData Name="layout_touch3" ActionTag="1931953109" VisibleForFrame="False" Tag="36" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="-21.0000" RightMargin="-21.0000" TopMargin="-8.0500" BottomMargin="-8.0500" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="642.0001" Y="39.1000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="300.0000" Y="11.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0700" Y="1.7000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="574.0000" Y="37.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5605" Y="0.3700" />
                <PreSize X="0.5859" Y="0.2300" />
                <BackGroundData Type="Normal" Path="mjstory/new_ui/image_audiobook_slider_background.png" Plist="" />
                <ProgressBarData Type="Normal" Path="mjstory/new_ui/image_audiobook_slider.png" Plist="" />
                <BallNormalData Type="Normal" Path="mjstory/new_ui/image_audiobook_slider_ball.png" Plist="" />
                <BallPressedData Type="Default" Path="Default/SliderNode_Press.png" Plist="" />
                <BallDisabledData Type="Default" Path="Default/SliderNode_Disable.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="time" ActionTag="1322692627" Tag="63" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="RightEdge" LeftMargin="275.0000" RightMargin="676.0000" TopMargin="22.0000" BottomMargin="49.0000" FontSize="24" LabelText="00:00" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="73.0000" Y="29.0000" />
                <AnchorPoint ScaleY="0.5000" />
                <Position X="275.0000" Y="63.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2686" Y="0.6350" />
                <PreSize X="0.0713" Y="0.2900" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="time_total" ActionTag="-1794390917" Tag="119" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="RightEdge" LeftMargin="811.0000" RightMargin="140.0000" TopMargin="22.0000" BottomMargin="49.0000" FontSize="24" LabelText="00:00" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="73.0000" Y="29.0000" />
                <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                <Position X="884.0000" Y="63.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8633" Y="0.6350" />
                <PreSize X="0.0713" Y="0.2900" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Panel_1" ActionTag="-618863071" Tag="120" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="RightEdge" LeftMargin="908.0000" RightMargin="115.0000" TopMargin="15.0000" BottomMargin="15.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="229" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="1.0000" Y="70.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="908.5000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8872" Y="0.5000" />
                <PreSize X="0.0010" Y="0.7000" />
                <SingleColor A="255" R="255" G="255" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="gradient" ActionTag="231018548" VisibleForFrame="False" Tag="138" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="-50.0000" BottomMargin="100.0000" LeftEage="16" RightEage="16" TopEage="16" BottomEage="16" Scale9OriginX="16" Scale9OriginY="16" Scale9Width="18" Scale9Height="18" ctype="ImageViewObjectData">
                <Size X="1024.0000" Y="50.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="512.0000" Y="100.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="1.0000" />
                <PreSize X="1.0000" Y="0.5000" />
                <FileData Type="Normal" Path="mjstory/new_ui/image_audiobook_gradient.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
            <Position X="512.0000" Y="-1.2288" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="-0.0016" />
            <PreSize X="1.0000" Y="0.1302" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="player_playlist" Visible="False" ActionTag="-100937640" Tag="54" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TopMargin="768.0000" BottomMargin="-100.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="100.0000" />
            <Children>
              <AbstractNodeData Name="button_favourite" ActionTag="562086370" CallBackType="Click" CallBackName="onFavourite" Tag="55" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="RightEdge" LeftMargin="924.0004" RightMargin="19.9996" TopMargin="15.0000" BottomMargin="5.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="-15" Scale9OriginY="-6" Scale9Width="30" Scale9Height="12" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="80.0000" Y="80.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="964.0004" Y="45.0000" />
                <Scale ScaleX="1.2500" ScaleY="1.2500" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9414" Y="0.4500" />
                <PreSize X="0.0781" Y="0.8000" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_play" ActionTag="-2133622040" CallBackType="Click" CallBackName="onPlayerPlay" Tag="56" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="195.0000" RightMargin="739.0000" TopMargin="5.0000" BottomMargin="5.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="231" Scale9Height="239" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="90.0000" Y="90.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="240.0000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2344" Y="0.5000" />
                <PreSize X="0.0879" Y="0.9000" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_audiobook_play_control.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_pause" ActionTag="-652921406" VisibleForFrame="False" CallBackType="Click" CallBackName="onPlayerPause" Tag="57" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="195.0000" RightMargin="739.0000" TopMargin="5.0000" BottomMargin="5.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="48" Scale9Height="56" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="90.0000" Y="90.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="240.0000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2344" Y="0.5000" />
                <PreSize X="0.0879" Y="0.9000" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_audiobook_pause.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="progress" ActionTag="1530787846" Tag="62" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="RightEdge" LeftMargin="374.0000" RightMargin="150.0000" TopMargin="51.5000" BottomMargin="25.5000" TouchEnable="True" PercentInfo="100" ctype="SliderObjectData">
                <Size X="500.0000" Y="23.0000" />
                <Children>
                  <AbstractNodeData Name="layout_touch3" ActionTag="-717457868" VisibleForFrame="False" Tag="63" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="-17.5000" RightMargin="-17.5000" TopMargin="-8.0500" BottomMargin="-8.0500" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="535.0000" Y="39.1000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="250.0000" Y="11.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0700" Y="1.7000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="624.0000" Y="37.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.6094" Y="0.3700" />
                <PreSize X="0.4883" Y="0.2300" />
                <BackGroundData Type="Normal" Path="mjstory/new_ui/image_audiobook_slider_background.png" Plist="" />
                <ProgressBarData Type="Normal" Path="mjstory/new_ui/image_audiobook_slider.png" Plist="" />
                <BallNormalData Type="Normal" Path="mjstory/new_ui/image_audiobook_slider_ball.png" Plist="" />
                <BallPressedData Type="Default" Path="Default/SliderNode_Press.png" Plist="" />
                <BallDisabledData Type="Default" Path="Default/SliderNode_Disable.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="time" ActionTag="-1319894332" Tag="64" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="RightEdge" LeftMargin="375.0000" RightMargin="576.0000" TopMargin="22.0000" BottomMargin="49.0000" FontSize="24" LabelText="00:00" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="73.0000" Y="29.0000" />
                <AnchorPoint ScaleY="0.5000" />
                <Position X="375.0000" Y="63.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3662" Y="0.6350" />
                <PreSize X="0.0713" Y="0.2900" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="time_total" ActionTag="1716185219" Tag="65" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="RightEdge" LeftMargin="811.0000" RightMargin="140.0000" TopMargin="22.0000" BottomMargin="49.0000" FontSize="24" LabelText="00:00" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="73.0000" Y="29.0000" />
                <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                <Position X="884.0000" Y="63.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8633" Y="0.6350" />
                <PreSize X="0.0713" Y="0.2900" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Panel_1" ActionTag="-204983221" Tag="66" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="RightEdge" LeftMargin="908.0000" RightMargin="115.0000" TopMargin="15.0000" BottomMargin="15.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="229" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="1.0000" Y="70.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="908.5000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8872" Y="0.5000" />
                <PreSize X="0.0010" Y="0.7000" />
                <SingleColor A="255" R="255" G="255" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="gradient" ActionTag="157146697" VisibleForFrame="False" Tag="67" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="-50.0000" BottomMargin="100.0000" LeftEage="16" RightEage="16" TopEage="16" BottomEage="16" Scale9OriginX="16" Scale9OriginY="16" Scale9Width="18" Scale9Height="18" ctype="ImageViewObjectData">
                <Size X="1024.0000" Y="50.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="512.0000" Y="100.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="1.0000" />
                <PreSize X="1.0000" Y="0.5000" />
                <FileData Type="Normal" Path="mjstory/new_ui/image_audiobook_gradient.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Panel_1_0" ActionTag="316498138" Tag="68" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="115.0000" RightMargin="908.0000" TopMargin="15.0000" BottomMargin="15.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="229" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="1.0000" Y="70.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="115.5000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1128" Y="0.5000" />
                <PreSize X="0.0010" Y="0.7000" />
                <SingleColor A="255" R="255" G="255" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_playlist" ActionTag="1377344132" CallBackType="Click" CallBackName="onPlaylist" Tag="69" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="20.0000" RightMargin="924.0000" TopMargin="10.0000" BottomMargin="10.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="15" Scale9OriginY="6" Scale9Width="50" Scale9Height="68" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="80.0000" Y="80.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="60.0000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0586" Y="0.5000" />
                <PreSize X="0.0781" Y="0.8000" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_playlist.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_prev" ActionTag="-693307748" CallBackType="Click" CallBackName="onPreviousTrack" Tag="70" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="127.0000" RightMargin="841.0000" TopMargin="22.0000" BottomMargin="22.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="26" Scale9Height="34" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="56.0000" Y="56.0000" />
                <Children>
                  <AbstractNodeData Name="layout_touch1" ActionTag="-1701296534" VisibleForFrame="False" Tag="71" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="56.0000" Y="56.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="28.0000" Y="28.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="155.0000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1514" Y="0.5000" />
                <PreSize X="0.0547" Y="0.5600" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_audiobook_back.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_next" ActionTag="-460346836" CallBackType="Click" CallBackName="onNextTrack" Tag="72" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="297.0000" RightMargin="671.0000" TopMargin="22.0000" BottomMargin="22.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="26" Scale9Height="34" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="56.0000" Y="56.0000" />
                <Children>
                  <AbstractNodeData Name="layout_touch2" ActionTag="-1589584833" VisibleForFrame="False" Tag="73" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="56.0000" Y="56.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="28.0000" Y="28.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="325.0000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3174" Y="0.5000" />
                <PreSize X="0.0547" Y="0.5600" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_audiobook_next.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
            <Position X="512.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" />
            <PreSize X="1.0000" Y="0.1302" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="thumb_container" ActionTag="727431442" Tag="179" IconVisible="False" LeftMargin="96.5000" RightMargin="662.5000" TopMargin="177.0000" BottomMargin="291.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="265.0000" Y="300.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="229.0000" Y="441.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.2236" Y="0.5742" />
            <PreSize X="0.2588" Y="0.3906" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="list_summary" ActionTag="1718731785" Tag="141" IconVisible="False" LeftMargin="421.0000" RightMargin="103.0000" TopMargin="175.0000" BottomMargin="353.0000" TouchEnable="True" ClipAble="True" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="0" DirectionType="Vertical" ctype="ListViewObjectData">
            <Size X="500.0000" Y="240.0000" />
            <Children>
              <AbstractNodeData Name="summary" ActionTag="306789805" Tag="180" IconVisible="False" BottomMargin="1.0000" IsCustomSize="True" FontSize="26" LabelText="There was once a very ugly creature with a very long tongue who lived in the forest. He was called Blue Goblin. He did not hurt anyone but the villagers were still scared of him. He felt very lonely." ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="500.0000" Y="239.0000" />
                <AnchorPoint ScaleY="1.0000" />
                <Position Y="240.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition Y="1.0000" />
                <PreSize X="1.0000" Y="0.9958" />
                <FontResource Type="Normal" Path="fonts/Vollkorn-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleY="1.0000" />
            <Position X="421.0000" Y="593.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.4111" Y="0.7721" />
            <PreSize X="0.4883" Y="0.3125" />
            <SingleColor A="255" R="150" G="150" B="255" />
            <FirstColor A="255" R="150" G="150" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="onboard_screensaver" Visible="False" ActionTag="952192134" Tag="67" IconVisible="False" PositionPercentXEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="769.0000" BottomMargin="-101.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="204" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="100.0000" />
            <Children>
              <AbstractNodeData Name="text_onboard_screensaver" ActionTag="709566450" Tag="68" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="289.0000" RightMargin="289.0000" TopMargin="38.0000" BottomMargin="38.0000" FontSize="20" LabelText="Bạn có thể nghe sách nói ở màn hình chờ" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="446.0000" Y="24.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.4355" Y="0.2400" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBoldItalic.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_close_onboard" ActionTag="804299437" Alpha="127" CallBackType="Click" CallBackName="onDismissOnboard" Tag="69" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="RightEdge" LeftMargin="924.0001" RightMargin="24.9999" TopMargin="12.5000" BottomMargin="12.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="53" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="75.0000" Y="75.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="961.5001" Y="50.0000" />
                <Scale ScaleX="0.7500" ScaleY="0.7500" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9390" Y="0.5000" />
                <PreSize X="0.0732" Y="0.7500" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_close.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" />
            <Position X="512.0000" Y="-101.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="-0.1315" />
            <PreSize X="1.0000" Y="0.1302" />
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>