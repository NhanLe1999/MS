<GameFile>
  <PropertyGroup Name="StoryParentEditUserInfo_Landscape" Type="Layer" ID="199e6f7c-e9f4-4e3b-a1be-48563c55441e" Version="3.10.0.0" />
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
      <ObjectData Name="layer" CustomClassName="StoryParentEditUserInfo_Landscape" Tag="31" ctype="GameLayerObjectData">
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
          <AbstractNodeData Name="bg_ngang" ActionTag="521911233" Tag="105" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="262.0000" RightMargin="262.0000" TopMargin="84.0000" BottomMargin="84.0000" TouchEnable="True" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="45" Scale9Height="45" ctype="ImageViewObjectData">
            <Size X="500.0000" Y="600.0000" />
            <Children>
              <AbstractNodeData Name="text1" ActionTag="364740023" Tag="307" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="60.0003" RightMargin="59.9997" TopMargin="59.8575" BottomMargin="503.1425" FontSize="30" LabelText="Nhập thông tin tài khoản" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="380.0000" Y="37.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="250.0003" Y="521.6425" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.8694" />
                <PreSize X="0.7600" Y="0.0617" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_done" ActionTag="-1508092092" CallBackType="Click" CallBackName="onSave" Tag="308" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="200.0004" RightMargin="199.9996" TopMargin="488.3579" BottomMargin="61.6421" TouchEnable="True" FontSize="24" ButtonText="Done" Scale9Enable="True" LeftEage="6" RightEage="6" TopEage="6" BottomEage="6" Scale9OriginX="6" Scale9OriginY="6" Scale9Width="8" Scale9Height="8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="100.0000" Y="50.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="250.0004" Y="86.6421" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1444" />
                <PreSize X="0.2000" Y="0.0833" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <TextColor A="255" R="255" G="255" B="255" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/parent/parent_notification_item_open_url.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="box_phone" ActionTag="-348836096" Tag="309" IconVisible="False" LeftMargin="40.0004" RightMargin="39.9996" TopMargin="356.3341" BottomMargin="143.6659" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="420.0000" Y="100.0000" />
                <Children>
                  <AbstractNodeData Name="title" ActionTag="1126840498" Tag="310" IconVisible="False" RightMargin="234.0000" TopMargin="3.0000" BottomMargin="63.0000" FontSize="28" LabelText="Số điện thoại" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="186.0000" Y="34.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position Y="80.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition Y="0.8000" />
                    <PreSize X="0.4429" Y="0.3400" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="box_textfield" ActionTag="-2107494207" Tag="311" IconVisible="False" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="40.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="51" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="420.0000" Y="60.0000" />
                    <Children>
                      <AbstractNodeData Name="textfield" ActionTag="-1611342128" Tag="312" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="10.0000" RightMargin="10.0000" TouchEnable="True" FontSize="24" IsCustomSize="True" LabelText="" PlaceHolderText="Nhập số điện thoại" MaxLengthText="10" ctype="TextFieldObjectData">
                        <Size X="400.0000" Y="60.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="10.0000" Y="30.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="77" G="77" B="77" />
                        <PrePosition X="0.0238" Y="0.5000" />
                        <PreSize X="0.9524" Y="1.0000" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="1.0000" Y="0.6000" />
                    <SingleColor A="255" R="0" G="0" B="0" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_error" ActionTag="137069972" VisibleForFrame="False" Tag="313" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="24.0000" RightMargin="24.0000" TopMargin="108.0000" BottomMargin="-28.0000" FontSize="16" LabelText="Đây là thông báo lỗi có thể rất dài như thế này" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="372.0000" Y="20.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="210.0000" Y="-18.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="186" G="0" B="0" />
                    <PrePosition X="0.5000" Y="-0.1800" />
                    <PreSize X="0.8857" Y="0.2000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="img_error" ActionTag="966015383" VisibleForFrame="False" Tag="314" IconVisible="False" LeftMargin="-15.0000" RightMargin="405.0000" TopMargin="102.0000" BottomMargin="-30.0000" LeftEage="59" RightEage="59" TopEage="53" BottomEage="53" Scale9OriginX="59" Scale9OriginY="53" Scale9Width="62" Scale9Height="56" ctype="ImageViewObjectData">
                    <Size X="30.0000" Y="28.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position Y="-16.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition Y="-0.1600" />
                    <PreSize X="0.0714" Y="0.2800" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/ap_wrong.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="40.0004" Y="143.6659" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0800" Y="0.2394" />
                <PreSize X="0.8400" Y="0.1667" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="box_email" ActionTag="-644716413" Tag="315" IconVisible="False" LeftMargin="40.0004" RightMargin="39.9996" TopMargin="236.3340" BottomMargin="263.6660" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="420.0000" Y="100.0000" />
                <Children>
                  <AbstractNodeData Name="img_error" ActionTag="789629471" VisibleForFrame="False" Tag="316" IconVisible="False" LeftMargin="-15.0000" RightMargin="405.0000" TopMargin="102.0000" BottomMargin="-30.0000" LeftEage="59" RightEage="59" TopEage="53" BottomEage="53" Scale9OriginX="59" Scale9OriginY="53" Scale9Width="62" Scale9Height="56" ctype="ImageViewObjectData">
                    <Size X="30.0000" Y="28.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position Y="-16.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition Y="-0.1600" />
                    <PreSize X="0.0714" Y="0.2800" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/ap_wrong.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_error" ActionTag="2075099189" VisibleForFrame="False" Tag="317" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="24.0000" RightMargin="24.0000" TopMargin="108.0000" BottomMargin="-28.0000" FontSize="16" LabelText="Đây là thông báo lỗi có thể rất dài như thế này" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="372.0000" Y="20.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="210.0000" Y="-18.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="186" G="0" B="0" />
                    <PrePosition X="0.5000" Y="-0.1800" />
                    <PreSize X="0.8857" Y="0.2000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="title" ActionTag="-963483633" Tag="318" IconVisible="False" RightMargin="338.0000" TopMargin="3.0000" BottomMargin="63.0000" FontSize="28" LabelText="Email" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="82.0000" Y="34.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position Y="80.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition Y="0.8000" />
                    <PreSize X="0.1952" Y="0.3400" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="box_textfield" ActionTag="-350709979" Tag="319" IconVisible="False" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="40.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="51" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="420.0000" Y="60.0000" />
                    <Children>
                      <AbstractNodeData Name="textfield" ActionTag="429659160" Tag="320" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="10.0000" RightMargin="10.0000" TouchEnable="True" FontSize="24" IsCustomSize="True" LabelText="" PlaceHolderText="Nhập email" MaxLengthText="10" ctype="TextFieldObjectData">
                        <Size X="400.0000" Y="60.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="10.0000" Y="30.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="77" G="77" B="77" />
                        <PrePosition X="0.0238" Y="0.5000" />
                        <PreSize X="0.9524" Y="1.0000" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="1.0000" Y="0.6000" />
                    <SingleColor A="255" R="0" G="0" B="0" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="40.0004" Y="263.6660" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0800" Y="0.4394" />
                <PreSize X="0.8400" Y="0.1667" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="box_address" ActionTag="341390934" VisibleForFrame="False" Tag="321" IconVisible="False" LeftMargin="40.0000" RightMargin="40.0000" TopMargin="100.0000" BottomMargin="400.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="420.0000" Y="100.0000" />
                <Children>
                  <AbstractNodeData Name="title" ActionTag="-1028059208" Tag="322" IconVisible="False" RightMargin="320.0000" TopMargin="3.0000" BottomMargin="63.0000" FontSize="28" LabelText="Địa chỉ" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="100.0000" Y="34.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position Y="80.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition Y="0.8000" />
                    <PreSize X="0.2381" Y="0.3400" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="box_textfield" ActionTag="-1510689842" Tag="323" IconVisible="False" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="40.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="51" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="420.0000" Y="60.0000" />
                    <Children>
                      <AbstractNodeData Name="textfield" ActionTag="-1220442778" Tag="324" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="10.0000" RightMargin="10.0000" TouchEnable="True" FontSize="24" IsCustomSize="True" LabelText="" PlaceHolderText="Nhập địa chỉ" MaxLengthText="10" ctype="TextFieldObjectData">
                        <Size X="400.0000" Y="60.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="10.0000" Y="30.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="77" G="77" B="77" />
                        <PrePosition X="0.0238" Y="0.5000" />
                        <PreSize X="0.9524" Y="1.0000" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="1.0000" Y="0.6000" />
                    <SingleColor A="255" R="0" G="0" B="0" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="img_error" ActionTag="-628802743" VisibleForFrame="False" Tag="325" IconVisible="False" LeftMargin="-5.0000" RightMargin="395.0000" TopMargin="102.0000" BottomMargin="-30.0000" LeftEage="59" RightEage="59" TopEage="53" BottomEage="53" Scale9OriginX="59" Scale9OriginY="53" Scale9Width="62" Scale9Height="56" ctype="ImageViewObjectData">
                    <Size X="30.0000" Y="28.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="10.0000" Y="-16.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0238" Y="-0.1600" />
                    <PreSize X="0.0714" Y="0.2800" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/ap_wrong.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_error" ActionTag="40970603" VisibleForFrame="False" Tag="326" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="24.0000" RightMargin="24.0000" TopMargin="108.0000" BottomMargin="-28.0000" FontSize="16" LabelText="Đây là thông báo lỗi có thể rất dài như thế này" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="372.0000" Y="20.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="210.0000" Y="-18.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="186" G="0" B="0" />
                    <PrePosition X="0.5000" Y="-0.1800" />
                    <PreSize X="0.8857" Y="0.2000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="40.0000" Y="400.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0800" Y="0.6667" />
                <PreSize X="0.8400" Y="0.1667" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="box_name" ActionTag="2047849453" Tag="327" IconVisible="False" LeftMargin="40.0004" RightMargin="39.9996" TopMargin="116.5673" BottomMargin="383.4327" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="420.0000" Y="100.0000" />
                <Children>
                  <AbstractNodeData Name="title" ActionTag="-725723797" Tag="328" IconVisible="False" RightMargin="194.0000" TopMargin="3.0000" BottomMargin="63.0000" FontSize="28" LabelText="Tên người dùng" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="226.0000" Y="34.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position Y="80.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition Y="0.8000" />
                    <PreSize X="0.5381" Y="0.3400" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="box_textfield" ActionTag="1389821959" Tag="329" IconVisible="False" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="40.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="51" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="420.0000" Y="60.0000" />
                    <Children>
                      <AbstractNodeData Name="textfield" ActionTag="-1501786796" Tag="330" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="10.0000" RightMargin="10.0000" TouchEnable="True" FontSize="24" IsCustomSize="True" LabelText="" PlaceHolderText="Nhập tên người dùng" MaxLengthText="10" ctype="TextFieldObjectData">
                        <Size X="400.0000" Y="60.0000" />
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="10.0000" Y="30.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="77" G="77" B="77" />
                        <PrePosition X="0.0238" Y="0.5000" />
                        <PreSize X="0.9524" Y="1.0000" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="1.0000" Y="0.6000" />
                    <SingleColor A="255" R="0" G="0" B="0" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="img_error" ActionTag="-1720382897" VisibleForFrame="False" Tag="331" IconVisible="False" LeftMargin="137.3398" RightMargin="252.6602" TopMargin="102.0000" BottomMargin="-30.0000" LeftEage="59" RightEage="59" TopEage="53" BottomEage="53" Scale9OriginX="59" Scale9OriginY="53" Scale9Width="62" Scale9Height="56" ctype="ImageViewObjectData">
                    <Size X="30.0000" Y="28.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="152.3398" Y="-16.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3627" Y="-0.1600" />
                    <PreSize X="0.0714" Y="0.2800" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/ap_wrong.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_error" ActionTag="789118369" VisibleForFrame="False" Tag="332" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="24.0000" RightMargin="24.0000" TopMargin="108.0000" BottomMargin="-28.0000" FontSize="16" LabelText="Đây là thông báo lỗi có thể rất dài như thế này" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="372.0000" Y="20.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="210.0000" Y="-18.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="222" G="0" B="0" />
                    <PrePosition X="0.5000" Y="-0.1800" />
                    <PreSize X="0.8857" Y="0.2000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="40.0004" Y="383.4327" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0800" Y="0.6391" />
                <PreSize X="0.8400" Y="0.1667" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_5" ActionTag="-1585296528" CallBackType="Click" CallBackName="onClose" Tag="132" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="467.4993" RightMargin="-14.4993" TopMargin="-11.5002" BottomMargin="564.5002" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="17" Scale9Height="25" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="47.0000" Y="47.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="490.9993" Y="588.0002" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9820" Y="0.9800" />
                <PreSize X="0.0940" Y="0.0783" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/popup/popup_ratebuy_btclose.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0984" ScaleY="1.0984" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.4883" Y="0.7813" />
            <FileData Type="Normal" Path="mjstory/dropdown/mj_level_background.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>