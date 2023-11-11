<GameFile>
  <PropertyGroup Name="NotificationPopup" Type="Layer" ID="1f83b56e-65ae-4503-9a17-527609f9f4c5" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="MSNotificationPopup" Tag="660" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="overlay" ActionTag="-941309833" Tag="674" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="229" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
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
          <AbstractNodeData Name="background" ActionTag="-2009030954" Tag="661" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="110.9760" RightMargin="113.0240" TopMargin="134.0000" BottomMargin="134.0000" Scale9Enable="True" LeftEage="60" RightEage="60" TopEage="40" BottomEage="40" Scale9OriginX="60" Scale9OriginY="40" Scale9Width="63" Scale9Height="44" ctype="ImageViewObjectData">
            <Size X="800.0000" Y="500.0000" />
            <Children>
              <AbstractNodeData Name="button_close" ActionTag="-800916367" CallBackType="Click" CallBackName="onClose" Tag="129" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="774.0000" RightMargin="-26.0000" TopMargin="-26.0000" BottomMargin="474.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="22" Scale9Height="30" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="52.0000" Y="52.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="800.0000" Y="500.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="1.0000" Y="1.0000" />
                <PreSize X="0.0650" Y="0.1040" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/pay/pay_guide_btclose.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="listview" ActionTag="-1622474134" Tag="126" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="TopEdge" LeftMargin="51.0400" RightMargin="48.9600" TopMargin="25.0000" BottomMargin="125.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="0" DirectionType="Vertical" ctype="ListViewObjectData">
                <Size X="700.0000" Y="350.0000" />
                <Children>
                  <AbstractNodeData Name="title" ActionTag="688491815" Tag="672" IconVisible="False" RightMargin="53.0000" BottomMargin="310.0000" FontSize="33" LabelText="Free to use Monkey Stories for 1 week" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="647.0000" Y="40.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="323.5000" Y="330.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.4621" Y="0.9429" />
                    <PreSize X="0.9243" Y="0.1143" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Panel_1" ActionTag="-1502208721" ZOrder="1" Tag="131" IconVisible="False" RightMargin="500.0000" TopMargin="40.0000" BottomMargin="290.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                    <Size X="200.0000" Y="20.0000" />
                    <AnchorPoint />
                    <Position Y="290.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition Y="0.8286" />
                    <PreSize X="0.2857" Y="0.0571" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="content" ActionTag="-268125536" ZOrder="2" Alpha="153" Tag="127" IconVisible="False" RightMargin="321.0000" TopMargin="60.0000" BottomMargin="260.0000" FontSize="25" LabelText="In the end, it's doesn't matter." ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="379.0000" Y="30.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="189.5000" Y="275.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.2707" Y="0.7857" />
                    <PreSize X="0.5414" Y="0.0857" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="401.0400" Y="475.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5013" Y="0.9500" />
                <PreSize X="0.8750" Y="0.7000" />
                <SingleColor A="255" R="150" G="150" B="255" />
                <FirstColor A="255" R="150" G="150" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_open" ActionTag="-1302646080" CallBackType="Click" CallBackName="onOpen" Tag="671" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="207.5000" RightMargin="207.5000" TopMargin="390.0000" BottomMargin="30.0000" TouchEnable="True" FontSize="28" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="5" Scale9Height="5" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="385.0000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="action" ActionTag="-1336185643" ZOrder="1" Tag="128" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="137.5000" RightMargin="137.5000" TopMargin="18.5000" BottomMargin="18.5000" FontSize="35" LabelText="OPEN" OutlineSize="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="110.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="192.5000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.2857" Y="0.5375" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Black.ttf" Plist="" />
                    <OutlineColor A="255" R="77" G="177" B="40" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="400.0000" Y="70.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1400" />
                <PreSize X="0.4812" Y="0.1600" />
                <FontResource Type="Normal" Path="fonts/IBMPlexSans-Bold.ttf" Plist="" />
                <TextColor A="255" R="255" G="255" B="255" />
                <PressedFileData Type="Normal" Path="mjstory/new_ui/button_intro_grade_blue.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_intro_grade_blue.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="510.9760" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.4990" Y="0.5000" />
            <PreSize X="0.7813" Y="0.6510" />
            <FileData Type="Normal" Path="mjstory/new_ui/frame_audiobook_content.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>