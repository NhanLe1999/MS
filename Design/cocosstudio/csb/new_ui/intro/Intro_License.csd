<GameFile>
  <PropertyGroup Name="Intro_License" Type="Layer" ID="ca91bf8d-b87d-4e22-82fe-094bda9f3f9b" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="MSIntroLicense" Tag="146" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="content" ActionTag="-1625375536" Tag="301" IconVisible="False" PositionPercentXEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="title" ActionTag="-1187481770" Tag="230" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="156.0000" RightMargin="156.0000" TopMargin="193.4000" BottomMargin="500.6000" FontSize="30" LabelText="Nếu bạn đã đăng ký mua và có mã kích hoạt,&#xA;vui lòng nhập tại đây" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="712.0000" Y="74.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="537.6000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.7000" />
                <PreSize X="0.6953" Y="0.0964" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="touch_layer" Visible="False" ActionTag="-1775365233" CallBackType="Touch" CallBackName="onTouch" Tag="240" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TopMargin="1.0676" BottomMargin="-1.0676" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="1024.0000" Y="768.0000" />
                <AnchorPoint />
                <Position Y="-1.0676" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition Y="-0.0014" />
                <PreSize X="1.0000" Y="1.0000" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_started" ActionTag="-1767858251" CallBackType="Click" CallBackName="onSend" Tag="248" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="387.0000" RightMargin="387.0000" TopMargin="459.2000" BottomMargin="228.8000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="355" Scale9Height="68" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="250.0000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="label_send" ActionTag="-2106949398" Tag="249" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="96.0000" RightMargin="96.0000" TopMargin="16.5000" BottomMargin="24.5000" FontSize="30" LabelText="Gửi" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="58.0000" Y="39.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="125.0000" Y="44.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5500" />
                    <PreSize X="0.2320" Y="0.4875" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Black.ttf" Plist="" />
                    <OutlineColor A="255" R="28" G="54" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="268.8000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.3500" />
                <PreSize X="0.2441" Y="0.1042" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="mjstory/new_ui/button_trial_start_pressed.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_trial_start.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="trigger" ActionTag="1553276224" CallBackType="Click" CallBackName="onTrigger" Tag="295" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="262.0000" RightMargin="262.0000" TopMargin="344.0000" BottomMargin="344.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="500.0000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="-1101396494" Tag="76" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="10.0000" BottomMargin="10.0000" Scale9Enable="True" LeftEage="23" RightEage="23" TopEage="26" BottomEage="26" Scale9OriginX="23" Scale9OriginY="26" Scale9Width="24" Scale9Height="28" ctype="ImageViewObjectData">
                    <Size X="500.0000" Y="60.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="250.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="0.7500" />
                    <FileData Type="Normal" Path="mjstory/new_ui/intro_letter.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="textfield" ActionTag="-1695682346" Tag="75" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="10.0000" BottomMargin="10.0000" TouchEnable="True" FontSize="35" IsCustomSize="True" LabelText="" PlaceHolderText="" MaxLengthText="10" ctype="TextFieldObjectData">
                    <Size X="500.0000" Y="60.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="250.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="0.7500" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-ExtraBold.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.2500" ScaleY="1.2500" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.4883" Y="0.1042" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="title_0" ActionTag="-804705413" VisibleForFrame="False" Tag="359" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="353.5000" RightMargin="353.5000" TopMargin="310.5600" BottomMargin="433.4400" FontSize="20" LabelText="Mã kích hoạt bao gồm 10 ký tự" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="317.0000" Y="24.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="445.4400" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5800" />
                <PreSize X="0.3096" Y="0.0313" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Italic.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="ListView_2" ActionTag="-664999747" Tag="361" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="247.2018" RightMargin="226.7982" TopMargin="804.6286" BottomMargin="-66.6286" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="0" VerticalType="Align_VerticalCenter" ctype="ListViewObjectData">
                <Size X="550.0000" Y="30.0000" />
                <Children>
                  <AbstractNodeData Name="Panel_17" ActionTag="-202435162" Tag="363" IconVisible="False" RightMargin="548.0000" TopMargin="-85.0000" BottomMargin="-85.0000" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="2.0000" Y="200.0000" />
                    <AnchorPoint />
                    <Position Y="-85.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition Y="-2.8333" />
                    <PreSize X="0.0036" Y="6.6667" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="522.2018" Y="-51.6286" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5100" Y="-0.0672" />
                <PreSize X="0.5371" Y="0.0391" />
                <SingleColor A="255" R="150" G="150" B="255" />
                <FirstColor A="255" R="150" G="150" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="intro.load.forgot" ActionTag="72084402" ZOrder="3" CallBackName="onSkip" Tag="110" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="404.0000" RightMargin="404.0000" TopMargin="564.9998" BottomMargin="179.0003" FontSize="20" LabelText="Quên mã kích hoạt?" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="216.0000" Y="24.0000" />
                <Children>
                  <AbstractNodeData Name="forgot_gachngang" ActionTag="1950974546" ZOrder="3" Tag="289" IconVisible="False" PositionPercentXEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="5.4000" RightMargin="5.4000" TopMargin="23.1400" BottomMargin="-0.1400" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="205.2000" Y="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                    <Position X="108.0000" Y="0.8600" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.0358" />
                    <PreSize X="0.9500" Y="0.0417" />
                    <SingleColor A="255" R="255" G="255" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="191.0003" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2487" />
                <PreSize X="0.2109" Y="0.0313" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_skip_0" ActionTag="-287873034" CallBackType="Click" CallBackName="onForgot" Tag="111" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="387.0000" RightMargin="387.0000" TopMargin="556.9998" BottomMargin="171.0002" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="-15" Scale9OriginY="-6" Scale9Width="30" Scale9Height="12" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="250.0000" Y="40.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="191.0002" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2487" />
                <PreSize X="0.2441" Y="0.0521" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="intro.load.skip" ActionTag="1161915884" CallBackName="onSkip" Tag="285" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="251.6345" RightMargin="254.3655" TopMargin="622.4524" BottomMargin="115.5476" FontSize="25" LabelText="Nếu bạn chưa có mã kích hoạt, vui lòng" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="518.0000" Y="30.0000" />
                <AnchorPoint ScaleX="0.4936" ScaleY="0.4795" />
                <Position X="507.2979" Y="129.9331" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="229" G="229" B="229" />
                <PrePosition X="0.4954" Y="0.1692" />
                <PreSize X="0.5059" Y="0.0391" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="intro.load.skip_0" ActionTag="471334704" CallBackName="onSkip" Tag="287" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="303.4999" RightMargin="303.5001" TopMargin="658.4909" BottomMargin="81.5091" FontSize="23" LabelText="vào đây để sử dụng bản miễn phí" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="417.0000" Y="28.0000" />
                <Children>
                  <AbstractNodeData Name="button_skip" ActionTag="1180174589" CallBackType="Click" CallBackName="onSkip" Tag="292" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="-11.6223" BottomMargin="-10.3777" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="-15" Scale9OriginY="-6" Scale9Width="30" Scale9Height="12" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="417.0000" Y="50.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="208.5000" Y="14.6223" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5222" />
                    <PreSize X="1.0000" Y="1.7857" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Panel_17_0" ActionTag="-386363437" ZOrder="3" Tag="288" IconVisible="False" PositionPercentXEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="10.4250" RightMargin="10.4250" TopMargin="27.1400" BottomMargin="-1.1400" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="396.1500" Y="2.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                    <Position X="208.5000" Y="0.8600" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.0307" />
                    <PreSize X="0.9500" Y="0.0714" />
                    <SingleColor A="255" R="255" G="255" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="511.9999" Y="95.5091" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1244" />
                <PreSize X="0.4072" Y="0.0365" />
                <FontResource Type="Normal" Path="fonts/Montserrat-BoldItalic.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" />
            <Position X="512.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>