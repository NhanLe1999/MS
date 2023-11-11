<GameFile>
  <PropertyGroup Name="StoryParentEditProfile_Landscape" Type="Layer" ID="f2c3a2d7-a079-4497-865a-fa8aca2239f6" Version="3.10.0.0" />
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
      <ObjectData Name="layer" CustomClassName="StoryParentEditProfile_Landscape" Tag="31" ctype="GameLayerObjectData">
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
          <AbstractNodeData Name="bg_ngang" ActionTag="521911233" Tag="105" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="51.2000" RightMargin="51.2000" TopMargin="38.4000" BottomMargin="38.4000" TouchEnable="True" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="45" Scale9Height="45" ctype="ImageViewObjectData">
            <Size X="921.6000" Y="691.2000" />
            <Children>
              <AbstractNodeData Name="btn_save" ActionTag="-1245059851" CallBackType="Click" CallBackName="onSave" Tag="83" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="385.8000" RightMargin="385.8000" TopMargin="620.5987" BottomMargin="10.6013" TouchEnable="True" FontSize="36" ButtonText="Lưu" Scale9Enable="True" LeftEage="6" RightEage="6" TopEage="6" BottomEage="6" Scale9OriginX="6" Scale9OriginY="6" Scale9Width="8" Scale9Height="8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="150.0000" Y="60.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="460.8000" Y="40.6013" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.0587" />
                <PreSize X="0.1628" Y="0.0868" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <TextColor A="255" R="255" G="255" B="255" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/parent/parent_notification_item_open_url.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="listview" ActionTag="-1676131041" Tag="84" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="401.3274" BottomMargin="89.8726" TouchEnable="True" ClipAble="True" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" IsBounceEnabled="True" ScrollDirectionType="0" VerticalType="Align_VerticalCenter" ctype="ListViewObjectData">
                <Size X="921.6000" Y="200.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="460.8000" Y="189.8726" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2747" />
                <PreSize X="1.0000" Y="0.2894" />
                <SingleColor A="255" R="150" G="150" B="255" />
                <FirstColor A="255" R="150" G="150" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="layout_name" ActionTag="-1129464335" Tag="85" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="30.0442" RightMargin="541.5558" TopMargin="131.2000" BottomMargin="460.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="350.0000" Y="100.0000" />
                <Children>
                  <AbstractNodeData Name="Text_16" ActionTag="-1064311668" Tag="86" IconVisible="False" PositionPercentYEnabled="True" RightMargin="164.0000" BottomMargin="63.0000" FontSize="30" LabelText="Tên con bạn" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="186.0000" Y="37.0000" />
                    <AnchorPoint ScaleY="1.0000" />
                    <Position Y="100.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition Y="1.0000" />
                    <PreSize X="0.5314" Y="0.3700" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Panel_15" ActionTag="-417168236" Alpha="51" Tag="87" IconVisible="False" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="40.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="350.0000" Y="60.0000" />
                    <AnchorPoint />
                    <Position />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition />
                    <PreSize X="1.0000" Y="0.6000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="textfield" ActionTag="1494759529" Tag="88" IconVisible="False" PercentWidthEnable="True" PercentWidthEnabled="True" HorizontalEdge="RightEdge" TopMargin="40.0000" TouchEnable="True" FontSize="26" IsCustomSize="True" LabelText="" PlaceHolderText="Nhập tên con của bạn" MaxLengthText="10" ctype="TextFieldObjectData">
                    <Size X="350.0000" Y="60.0000" />
                    <AnchorPoint />
                    <Position />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="77" G="77" B="77" />
                    <PrePosition />
                    <PreSize X="1.0000" Y="0.6000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="img_error" ActionTag="-1126125455" VisibleForFrame="False" Tag="199" IconVisible="False" RightMargin="320.0000" TopMargin="102.0000" BottomMargin="-30.0000" LeftEage="59" RightEage="59" TopEage="53" BottomEage="53" Scale9OriginX="59" Scale9OriginY="53" Scale9Width="62" Scale9Height="56" ctype="ImageViewObjectData">
                    <Size X="30.0000" Y="28.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="15.0000" Y="-16.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0429" Y="-0.1600" />
                    <PreSize X="0.0857" Y="0.2800" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/ap_wrong.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_error" ActionTag="-1306830352" VisibleForFrame="False" Tag="198" IconVisible="False" LeftMargin="35.0000" RightMargin="71.0000" TopMargin="108.0000" BottomMargin="-28.0000" FontSize="16" LabelText="Bạn phải nhập tên để tiếp tục" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="244.0000" Y="20.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="35.0000" Y="-18.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="186" G="0" B="0" />
                    <PrePosition X="0.1000" Y="-0.1800" />
                    <PreSize X="0.6971" Y="0.2000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="0.5000" />
                <Position X="30.0442" Y="510.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0326" Y="0.7378" />
                <PreSize X="0.3798" Y="0.1447" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="layout_gender" ActionTag="1754113619" Tag="89" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="30.0442" RightMargin="491.5558" TopMargin="261.2000" BottomMargin="350.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="400.0000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="Panel_2" ActionTag="-957631117" Tag="90" IconVisible="False" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="139.0000" RightMargin="259.0000" TopMargin="8.0000" BottomMargin="8.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="2.0000" Y="64.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="140.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3500" Y="0.5000" />
                    <PreSize X="0.0050" Y="0.8000" />
                    <SingleColor A="255" R="0" G="0" B="0" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="gender_label" ActionTag="-252992629" Tag="91" IconVisible="False" PositionPercentYEnabled="True" RightMargin="273.0000" TopMargin="21.5000" BottomMargin="21.5000" FontSize="30" LabelText="Giới tính" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="127.0000" Y="37.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="136" G="136" B="136" />
                    <PrePosition Y="0.5000" />
                    <PreSize X="0.3175" Y="0.4625" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="icon_boy" ActionTag="1084714981" CallBackType="Click" CallBackName="onSelectBoy" Tag="92" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="265.0000" RightMargin="75.0000" TopMargin="10.0000" BottomMargin="10.0000" TouchEnable="True" LeftEage="49" RightEage="49" TopEage="33" BottomEage="33" Scale9OriginX="49" Scale9OriginY="33" Scale9Width="171" Scale9Height="202" ctype="ImageViewObjectData">
                    <Size X="60.0000" Y="60.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="295.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7375" Y="0.5000" />
                    <PreSize X="0.1500" Y="0.7500" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/male.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="icon_girl" ActionTag="-1281013828" CallBackType="Click" CallBackName="onSelectGirl" Tag="93" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="175.0000" RightMargin="165.0000" TopMargin="10.0000" BottomMargin="10.0000" TouchEnable="True" LeftEage="49" RightEage="49" TopEage="33" BottomEage="33" Scale9OriginX="49" Scale9OriginY="33" Scale9Width="190" Scale9Height="221" ctype="ImageViewObjectData">
                    <Size X="60.0000" Y="60.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="205.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5125" Y="0.5000" />
                    <PreSize X="0.1500" Y="0.7500" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/female.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="img_error" ActionTag="-987613557" VisibleForFrame="False" Tag="200" IconVisible="False" RightMargin="370.0000" TopMargin="82.0000" BottomMargin="-30.0000" LeftEage="59" RightEage="59" TopEage="53" BottomEage="53" Scale9OriginX="59" Scale9OriginY="53" Scale9Width="62" Scale9Height="56" ctype="ImageViewObjectData">
                    <Size X="30.0000" Y="28.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="15.0000" Y="-16.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0375" Y="-0.2000" />
                    <PreSize X="0.0750" Y="0.3500" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/ap_wrong.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_error" ActionTag="1818067957" VisibleForFrame="False" Tag="201" IconVisible="False" LeftMargin="35.0000" RightMargin="-7.0000" TopMargin="88.0000" BottomMargin="-28.0000" FontSize="16" LabelText="Đây là thông báo lỗi có thể rất dài như thế này" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="372.0000" Y="20.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="35.0000" Y="-18.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="186" G="0" B="0" />
                    <PrePosition X="0.0875" Y="-0.2250" />
                    <PreSize X="0.9300" Y="0.2500" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="0.5000" />
                <Position X="30.0442" Y="390.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0326" Y="0.5642" />
                <PreSize X="0.4340" Y="0.1157" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="layout_age" ActionTag="-1852226550" Tag="94" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="421.6000" TopMargin="126.2000" BottomMargin="355.0000" TouchEnable="True" ClipAble="False" ColorAngle="90.0000" Scale9Enable="True" LeftEage="60" RightEage="60" TopEage="60" BottomEage="60" Scale9OriginX="60" Scale9OriginY="60" Scale9Width="5" Scale9Height="4" ctype="PanelObjectData">
                <Size X="500.0000" Y="210.0000" />
                <Children>
                  <AbstractNodeData Name="Panel_2_0" ActionTag="-1867411231" Tag="95" IconVisible="False" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="133.6300" RightMargin="364.3700" TopMargin="33.1170" BottomMargin="29.8830" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="2.0000" Y="147.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="134.6300" Y="103.3830" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2693" Y="0.4923" />
                    <PreSize X="0.0040" Y="0.7000" />
                    <SingleColor A="255" R="0" G="0" B="0" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_age" ActionTag="-2091191398" Tag="96" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="40.0000" RightMargin="387.0000" TopMargin="83.5000" BottomMargin="83.5000" FontSize="35" LabelText="Age" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="73.0000" Y="43.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="40.0000" Y="105.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="38" G="176" B="230" />
                    <PrePosition X="0.0800" Y="0.5000" />
                    <PreSize X="0.1460" Y="0.2048" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="age_2" ActionTag="863375097" CallBackType="Click" CallBackName="onSelectAge" Tag="2" IconVisible="False" LeftMargin="141.5958" RightMargin="283.4042" TopMargin="50.1309" BottomMargin="109.8691" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Enable="True" LeftEage="117" RightEage="117" TopEage="60" BottomEage="60" Scale9OriginX="117" Scale9OriginY="60" Scale9Width="122" Scale9Height="62" ctype="PanelObjectData">
                    <Size X="75.0000" Y="50.0000" />
                    <Children>
                      <AbstractNodeData Name="Text_1_0_0" ActionTag="1382667758" Tag="98" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="11.5000" RightMargin="11.5000" TopMargin="3.0000" BottomMargin="3.0000" FontSize="36" LabelText="&lt; 3" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="52.0000" Y="44.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="37.5000" Y="25.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="38" G="176" B="230" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.6933" Y="0.8800" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="141.5958" Y="109.8691" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2832" Y="0.5232" />
                    <PreSize X="0.1500" Y="0.2381" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/button age 2.png" Plist="" />
                    <SingleColor A="255" R="38" G="176" B="230" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="age_3" ActionTag="-1830703667" CallBackType="Click" CallBackName="onSelectAge" Tag="3" IconVisible="False" LeftMargin="226.3859" RightMargin="198.6141" TopMargin="49.8407" BottomMargin="110.1593" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Enable="True" LeftEage="117" RightEage="117" TopEage="60" BottomEage="60" Scale9OriginX="117" Scale9OriginY="60" Scale9Width="122" Scale9Height="62" ctype="PanelObjectData">
                    <Size X="75.0000" Y="50.0000" />
                    <Children>
                      <AbstractNodeData Name="Text_1_0_0" ActionTag="1790412613" Tag="100" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="26.5000" RightMargin="26.5000" TopMargin="3.0000" BottomMargin="3.0000" FontSize="36" LabelText="3" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="22.0000" Y="44.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="37.5000" Y="25.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="38" G="176" B="230" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.2933" Y="0.8800" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="226.3859" Y="110.1593" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4528" Y="0.5246" />
                    <PreSize X="0.1500" Y="0.2381" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/button age 2.png" Plist="" />
                    <SingleColor A="255" R="38" G="176" B="230" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="age_4" ActionTag="-921760316" CallBackType="Click" CallBackName="onSelectAge" Tag="4" IconVisible="False" LeftMargin="309.1982" RightMargin="115.8018" TopMargin="49.5221" BottomMargin="110.4779" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Enable="True" LeftEage="117" RightEage="117" TopEage="60" BottomEage="60" Scale9OriginX="117" Scale9OriginY="60" Scale9Width="122" Scale9Height="62" ctype="PanelObjectData">
                    <Size X="75.0000" Y="50.0000" />
                    <Children>
                      <AbstractNodeData Name="Text_1_0_0" ActionTag="930419146" Tag="102" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="24.0000" RightMargin="24.0000" TopMargin="3.0000" BottomMargin="3.0000" FontSize="36" LabelText="4" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="27.0000" Y="44.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="37.5000" Y="25.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="38" G="176" B="230" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.3600" Y="0.8800" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="309.1982" Y="110.4779" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.6184" Y="0.5261" />
                    <PreSize X="0.1500" Y="0.2381" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/button age 2.png" Plist="" />
                    <SingleColor A="255" R="38" G="176" B="230" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="age_5" ActionTag="-435264114" CallBackType="Click" CallBackName="onSelectAge" Tag="5" IconVisible="False" LeftMargin="391.3192" RightMargin="33.6808" TopMargin="49.9461" BottomMargin="110.0539" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Enable="True" LeftEage="117" RightEage="117" TopEage="60" BottomEage="60" Scale9OriginX="117" Scale9OriginY="60" Scale9Width="122" Scale9Height="62" ctype="PanelObjectData">
                    <Size X="75.0000" Y="50.0000" />
                    <Children>
                      <AbstractNodeData Name="Text_1_0_0" ActionTag="365042718" Tag="104" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="26.5000" RightMargin="26.5000" TopMargin="3.0000" BottomMargin="3.0000" FontSize="36" LabelText="5" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="22.0000" Y="44.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="37.5000" Y="25.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="38" G="176" B="230" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.2933" Y="0.8800" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="391.3192" Y="110.0539" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7826" Y="0.5241" />
                    <PreSize X="0.1500" Y="0.2381" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/button age 2.png" Plist="" />
                    <SingleColor A="255" R="38" G="176" B="230" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="age_6" ActionTag="-1658774814" CallBackType="Click" CallBackName="onSelectAge" Tag="6" IconVisible="False" LeftMargin="143.0669" RightMargin="281.9331" TopMargin="117.8505" BottomMargin="42.1495" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Enable="True" LeftEage="117" RightEage="117" TopEage="60" BottomEage="60" Scale9OriginX="117" Scale9OriginY="60" Scale9Width="122" Scale9Height="62" ctype="PanelObjectData">
                    <Size X="75.0000" Y="50.0000" />
                    <Children>
                      <AbstractNodeData Name="Text_1_0_0" ActionTag="57171149" Tag="106" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="25.5000" RightMargin="25.5000" TopMargin="3.0000" BottomMargin="3.0000" FontSize="36" LabelText="6" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="24.0000" Y="44.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="37.5000" Y="25.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="38" G="176" B="230" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.3200" Y="0.8800" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="143.0669" Y="42.1495" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2861" Y="0.2007" />
                    <PreSize X="0.1500" Y="0.2381" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/button age 2.png" Plist="" />
                    <SingleColor A="255" R="38" G="176" B="230" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="age_7" ActionTag="364423540" CallBackType="Click" CallBackName="onSelectAge" Tag="7" IconVisible="False" LeftMargin="227.2968" RightMargin="197.7032" TopMargin="117.5324" BottomMargin="42.4676" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Enable="True" LeftEage="117" RightEage="117" TopEage="60" BottomEage="60" Scale9OriginX="117" Scale9OriginY="60" Scale9Width="122" Scale9Height="62" ctype="PanelObjectData">
                    <Size X="75.0000" Y="50.0000" />
                    <Children>
                      <AbstractNodeData Name="Text_1_0_0" ActionTag="1331413615" Tag="108" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="26.0000" RightMargin="26.0000" TopMargin="3.0000" BottomMargin="3.0000" FontSize="36" LabelText="7" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="23.0000" Y="44.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="37.5000" Y="25.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="38" G="176" B="230" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.3067" Y="0.8800" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="227.2968" Y="42.4676" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4546" Y="0.2022" />
                    <PreSize X="0.1500" Y="0.2381" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/button age 2.png" Plist="" />
                    <SingleColor A="255" R="38" G="176" B="230" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="age_8" ActionTag="-431506534" CallBackType="Click" CallBackName="onSelectAge" Tag="8" IconVisible="False" LeftMargin="310.0714" RightMargin="114.9286" TopMargin="116.6118" BottomMargin="43.3882" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Enable="True" LeftEage="117" RightEage="117" TopEage="60" BottomEage="60" Scale9OriginX="117" Scale9OriginY="60" Scale9Width="122" Scale9Height="62" ctype="PanelObjectData">
                    <Size X="75.0000" Y="50.0000" />
                    <Children>
                      <AbstractNodeData Name="Text_1_0_0" ActionTag="-1240845241" Tag="110" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="25.0000" RightMargin="25.0000" TopMargin="3.0000" BottomMargin="3.0000" FontSize="36" LabelText="8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="25.0000" Y="44.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="37.5000" Y="25.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="38" G="176" B="230" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.3333" Y="0.8800" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="310.0714" Y="43.3882" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.6201" Y="0.2066" />
                    <PreSize X="0.1500" Y="0.2381" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/button age 2.png" Plist="" />
                    <SingleColor A="255" R="38" G="176" B="230" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="age_9" ActionTag="1182902499" CallBackType="Click" CallBackName="onSelectAge" Tag="9" IconVisible="False" LeftMargin="393.6154" RightMargin="31.3846" TopMargin="116.3212" BottomMargin="43.6788" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Enable="True" LeftEage="117" RightEage="117" TopEage="60" BottomEage="60" Scale9OriginX="117" Scale9OriginY="60" Scale9Width="122" Scale9Height="62" ctype="PanelObjectData">
                    <Size X="75.0000" Y="50.0000" />
                    <Children>
                      <AbstractNodeData Name="Text_1_0_0" ActionTag="-1218684878" Tag="112" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="14.5000" RightMargin="14.5000" TopMargin="3.0000" BottomMargin="3.0000" FontSize="36" LabelText="&gt;8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="46.0000" Y="44.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="37.5000" Y="25.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="38" G="176" B="230" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.6133" Y="0.8800" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="393.6154" Y="43.6788" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7872" Y="0.2080" />
                    <PreSize X="0.1500" Y="0.2381" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/button age 2.png" Plist="" />
                    <SingleColor A="255" R="38" G="176" B="230" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_error" ActionTag="2065199015" VisibleForFrame="False" Tag="203" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="126.5000" RightMargin="126.5000" TopMargin="200.0000" BottomMargin="-10.0000" FontSize="16" LabelText="Bạn phải chọn tuổi để tiếp tục" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="247.0000" Y="20.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="250.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="186" G="0" B="0" />
                    <PrePosition X="0.5000" />
                    <PreSize X="0.4940" Y="0.0952" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="img_error" ActionTag="-948719499" VisibleForFrame="False" Tag="202" IconVisible="False" LeftMargin="35.0000" RightMargin="435.0000" TopMargin="196.0000" BottomMargin="-14.0000" LeftEage="59" RightEage="59" TopEage="53" BottomEage="53" Scale9OriginX="59" Scale9OriginY="53" Scale9Width="62" Scale9Height="56" ctype="ImageViewObjectData">
                    <Size X="30.0000" Y="28.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="50.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1000" />
                    <PreSize X="0.0600" Y="0.1333" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/ap_wrong.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                <Position X="921.6000" Y="460.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="1.0000" Y="0.6655" />
                <PreSize X="0.5425" Y="0.3038" />
                <FileData Type="Normal" Path="mjstory/new_ui/login/ap_profile_background.png" Plist="" />
                <SingleColor A="255" R="255" G="255" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="header" ActionTag="485827049" Tag="113" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" BottomMargin="611.2000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="921.6000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1" ActionTag="346659697" Tag="114" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="261.8000" RightMargin="261.8000" TopMargin="15.5000" BottomMargin="15.5000" FontSize="40" LabelText="Chỉnh sửa thông tin" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="398.0000" Y="49.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="460.8000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.4319" Y="0.6125" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btn_close" ActionTag="-1640209626" CallBackType="Click" CallBackName="onClose" Tag="115" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="845.5200" RightMargin="16.0800" TopMargin="10.0000" BottomMargin="10.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="30" Scale9Height="38" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="60.0000" Y="60.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="875.5200" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9500" Y="0.5000" />
                    <PreSize X="0.0651" Y="0.7500" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/button_sidebar_close.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="1.0000" />
                <Position Y="691.2000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition Y="1.0000" />
                <PreSize X="1.0000" Y="0.1157" />
                <SingleColor A="255" R="41" G="176" B="223" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="0.0100" ScaleY="0.0100" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.9000" Y="0.9000" />
            <FileData Type="Normal" Path="mjstory/dropdown/mj_level_background.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>