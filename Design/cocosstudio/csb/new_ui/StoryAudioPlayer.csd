<GameFile>
  <PropertyGroup Name="StoryAudioPlayer" Type="Layer" ID="194f6450-53cf-4d56-a79b-fe0234aedb00" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="MSStoryAudioPlayer" Tag="64" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="Panel_3" ActionTag="187475145" Tag="73" IconVisible="False" PositionPercentXEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="460.8000" RightMargin="51.2000" TouchEnable="True" ClipAble="False" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="512.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="panel_controller" ActionTag="1594876335" Tag="85" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="385.8000" BottomMargin="232.2000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="512.0000" Y="150.0000" />
                <Children>
                  <AbstractNodeData Name="button_play" ActionTag="27616403" CallBackType="Click" CallBackName="onPlay" Tag="68" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="181.0000" RightMargin="181.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="120" Scale9Height="128" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="150.0000" Y="150.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="256.0000" Y="75.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.2930" Y="1.0000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/qr_button_play.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="label_play" ActionTag="1685702637" Tag="99" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="197.0000" RightMargin="197.0000" TopMargin="163.0000" BottomMargin="-37.0000" FontSize="20" LabelText="Đọc cho bé" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="118.0000" Y="24.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="256.0000" Y="-25.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="-0.1667" />
                    <PreSize X="0.2305" Y="0.1600" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="button_pause" ActionTag="1522511297" CallBackType="Click" CallBackName="onPause" Tag="75" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="181.0000" RightMargin="181.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="120" Scale9Height="128" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="150.0000" Y="150.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="256.0000" Y="75.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.2930" Y="1.0000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/qr_button_pause.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="button_back_page" ActionTag="-305273393" CallBackType="Click" CallBackName="onPrevPage" Tag="83" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="52.4000" RightMargin="359.6000" TopMargin="25.0000" BottomMargin="25.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="120" Scale9Height="128" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="100.0000" Y="100.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="102.4000" Y="75.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2000" Y="0.5000" />
                    <PreSize X="0.1953" Y="0.6667" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/qr_button_prev_page.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="button_next_page" ActionTag="817707067" CallBackType="Click" CallBackName="onNextPage" Tag="84" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="359.6000" RightMargin="52.4000" TopMargin="25.0000" BottomMargin="25.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="120" Scale9Height="128" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="100.0000" Y="100.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="409.6000" Y="75.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8000" Y="0.5000" />
                    <PreSize X="0.1953" Y="0.6667" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/qr_button_next_page.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="label_prev_page" ActionTag="58926711" Tag="100" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="41.4000" RightMargin="348.6000" TopMargin="163.0000" BottomMargin="-37.0000" FontSize="20" LabelText="Trang trước" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="122.0000" Y="24.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="102.4000" Y="-25.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2000" Y="-0.1667" />
                    <PreSize X="0.2383" Y="0.1600" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="label_next_page" ActionTag="409585132" Tag="101" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="358.6000" RightMargin="51.4000" TopMargin="163.0000" BottomMargin="-37.0000" FontSize="20" LabelText="Trang sau" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="102.0000" Y="24.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="409.6000" Y="-25.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8000" Y="-0.1667" />
                    <PreSize X="0.1992" Y="0.1600" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="256.0000" Y="307.2000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4000" />
                <PreSize X="1.0000" Y="0.1953" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_download" Visible="False" ActionTag="2080399098" CallBackType="Click" CallBackName="onDownload" Tag="70" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="181.0000" RightMargin="181.0000" TopMargin="385.8000" BottomMargin="232.2000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="120" Scale9Height="128" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="150.0000" Y="150.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="256.0000" Y="307.2000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4000" />
                <PreSize X="0.2930" Y="0.1953" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/qr_button_download.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="panel_progress" Visible="False" ActionTag="-376717516" Tag="192" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="56.0000" RightMargin="56.0000" TopMargin="310.8000" BottomMargin="157.2000" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="400.0000" Y="300.0000" />
                <Children>
                  <AbstractNodeData Name="Image_3" ActionTag="2109524296" Tag="193" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="124.5000" RightMargin="124.5000" TopMargin="74.5000" BottomMargin="74.5000" LeftEage="49" RightEage="49" TopEage="49" BottomEage="49" Scale9OriginX="49" Scale9OriginY="49" Scale9Width="53" Scale9Height="53" ctype="ImageViewObjectData">
                    <Size X="151.0000" Y="151.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.0000" Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.3775" Y="0.5033" />
                    <FileData Type="Normal" Path="mjstory/new_ui/qr_loading_background.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="loading_bar" ActionTag="-206730070" Tag="194" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="135.0000" RightMargin="135.0000" TopMargin="85.0000" BottomMargin="85.0000" ProgressInfo="0" ctype="LoadingBarObjectData">
                    <Size X="130.0000" Y="130.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.0000" Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.3250" Y="0.4333" />
                    <ImageFileData Type="Normal" Path="mjstory/new_ui/qr_loading_bar.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_status" ActionTag="-1482440662" VisibleForFrame="False" Tag="196" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="104.5000" RightMargin="104.5000" TopMargin="135.0800" BottomMargin="127.9200" FontSize="30" LabelText="Please wait..." HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="191.0000" Y="37.0000" />
                    <AnchorPoint ScaleX="0.5000" />
                    <Position X="200.0000" Y="127.9200" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="108" G="146" B="244" />
                    <PrePosition X="0.5000" Y="0.4264" />
                    <PreSize X="0.4775" Y="0.1233" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_percent" ActionTag="-1688501453" Tag="197" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="173.0000" RightMargin="173.0000" TopMargin="129.5000" BottomMargin="129.5000" FontSize="30" LabelText="0%" HorizontalAlignmentType="HT_Center" OutlineSize="2" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="54.0000" Y="41.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="200.0000" Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.1350" Y="0.1367" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Black.ttf" Plist="" />
                    <OutlineColor A="255" R="60" G="135" B="37" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="256.0000" Y="307.2000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4000" />
                <PreSize X="0.7813" Y="0.3906" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="panel_indicator" ActionTag="2017342979" Tag="92" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="56.0000" RightMargin="56.0000" TopMargin="261.3000" BottomMargin="491.7000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="400.0000" Y="15.0000" />
                <Children>
                  <AbstractNodeData Name="background" ActionTag="1198137703" Tag="93" IconVisible="False" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" Scale9Enable="True" LeftEage="9" RightEage="9" TopEage="4" BottomEage="4" Scale9OriginX="9" Scale9OriginY="4" Scale9Width="12" Scale9Height="7" ctype="ImageViewObjectData">
                    <Size X="400.0000" Y="15.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position Y="7.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <FileData Type="Normal" Path="mjstory/new_ui/qr_bar.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="256.0000" Y="499.2000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6500" />
                <PreSize X="0.7813" Y="0.0195" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position X="460.8000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.4500" />
            <PreSize X="0.5000" Y="1.0000" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="button_close" ActionTag="-1324259157" CallBackType="Click" CallBackName="onBack" Tag="71" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="TopEdge" LeftMargin="939.0000" RightMargin="10.0000" TopMargin="10.0000" BottomMargin="683.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="53" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="75.0000" Y="75.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="976.5000" Y="720.5000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.9536" Y="0.9382" />
            <PreSize X="0.0732" Y="0.0977" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="Normal" Path="mjstory/papgemenucontrol_btclose.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="Panel_1" ActionTag="-1177825054" Tag="66" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" RightMargin="512.0000" TouchEnable="True" ClipAble="False" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="512.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="placeholder" ActionTag="742062846" Tag="72" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="86.0000" RightMargin="86.0000" TopMargin="77.2000" BottomMargin="230.8000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="340.0000" Y="460.0000" />
                <Children>
                  <AbstractNodeData Name="story_title" ActionTag="1848634916" Tag="69" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="50.0000" RightMargin="49.9940" TopMargin="50.0000" BottomMargin="50.0000" StretchWidthEnable="True" IsCustomSize="True" FontSize="35" LabelText="Text Label" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="240.0060" Y="360.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="170.0030" Y="230.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.7059" Y="0.7826" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="256.0000" Y="460.8000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6000" />
                <PreSize X="0.6641" Y="0.5990" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="lb_level" ActionTag="152945113" Tag="86" IconVisible="False" LeftMargin="160.0000" RightMargin="237.0000" TopMargin="569.5001" BottomMargin="161.4999" FontSize="30" LabelText="Cấp độ:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="115.0000" Y="37.0000" />
                <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                <Position X="275.0000" Y="179.9999" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5371" Y="0.2344" />
                <PreSize X="0.2246" Y="0.0482" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="lb_lexile" ActionTag="-998943208" Tag="87" IconVisible="False" LeftMargin="90.0000" RightMargin="237.0000" TopMargin="619.5000" BottomMargin="111.5000" FontSize="30" LabelText="Điểm Lexile:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="185.0000" Y="37.0000" />
                <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                <Position X="275.0000" Y="130.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5371" Y="0.1693" />
                <PreSize X="0.3613" Y="0.0482" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="lb_page" ActionTag="1304544997" Tag="88" IconVisible="False" LeftMargin="139.0000" RightMargin="237.0000" TopMargin="669.5000" BottomMargin="61.5000" FontSize="30" LabelText="Số trang:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="136.0000" Y="37.0000" />
                <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                <Position X="275.0000" Y="80.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5371" Y="0.1042" />
                <PreSize X="0.2656" Y="0.0482" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="text_level" ActionTag="2000988296" Tag="89" IconVisible="False" LeftMargin="300.0000" RightMargin="185.0000" TopMargin="569.5001" BottomMargin="161.4999" FontSize="30" LabelText="A" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="27.0000" Y="37.0000" />
                <AnchorPoint ScaleY="0.5000" />
                <Position X="300.0000" Y="179.9999" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5859" Y="0.2344" />
                <PreSize X="0.0527" Y="0.0482" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="text_lexile" ActionTag="-1113838053" Tag="90" IconVisible="False" LeftMargin="300.0000" RightMargin="139.0000" TopMargin="619.5000" BottomMargin="111.5000" FontSize="30" LabelText="100L" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="73.0000" Y="37.0000" />
                <AnchorPoint ScaleY="0.5000" />
                <Position X="300.0000" Y="130.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5859" Y="0.1693" />
                <PreSize X="0.1426" Y="0.0482" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="text_page_count" ActionTag="1527755793" Tag="91" IconVisible="False" LeftMargin="300.0000" RightMargin="177.0000" TopMargin="669.5000" BottomMargin="61.5000" FontSize="30" LabelText="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="35.0000" Y="37.0000" />
                <AnchorPoint ScaleY="0.5000" />
                <Position X="300.0000" Y="80.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5859" Y="0.1042" />
                <PreSize X="0.0684" Y="0.0482" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.5000" Y="1.0000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>