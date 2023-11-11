<GameFile>
  <PropertyGroup Name="MaterialEnterEmail" Type="Layer" ID="12dfa589-3e60-45c9-9800-3e0a75459a17" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="90" Speed="1.0000" ActivedAnimationName="hide">
        <Timeline ActionTag="-1526135579" Property="Alpha">
          <IntFrame FrameIndex="0" Value="229">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="30" Value="204">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
        <Timeline ActionTag="794197901" Property="Scale">
          <ScaleFrame FrameIndex="60" X="1.0000" Y="1.0000">
            <EasingData Type="25" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="90" X="0.0100" Y="0.0100">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="1153921209" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.0100" Y="0.0100">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="1.1000" Y="1.1000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="1.1000" Y="1.1000">
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
      <ObjectData Name="layer" CustomClassName="MSMaterialEnterEmail" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="Panel_1" ActionTag="-1526135579" Alpha="229" Tag="93" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="203" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
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
          <AbstractNodeData Name="alert_layout" ActionTag="1153921209" Tag="25" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="310.5000" RightMargin="310.5000" TopMargin="239.9300" BottomMargin="130.0700" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="403.0000" Y="398.0000" />
            <Children>
              <AbstractNodeData Name="alert_bg" ActionTag="521911233" Tag="105" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-98.5000" RightMargin="-98.5000" TopMargin="-175.9888" BottomMargin="-76.0112" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="570" Scale9Height="620" ctype="ImageViewObjectData">
                <Size X="600.0000" Y="650.0000" />
                <Children>
                  <AbstractNodeData Name="desc_1" ActionTag="100557992" Tag="6" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="60.0000" RightMargin="60.0000" TopMargin="253.0000" BottomMargin="277.0000" IsCustomSize="True" FontSize="24" LabelText="Silakan Bapak/Ibu konfirmasi alamat email untuk mendapat materi pembelajaran:" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="480.0000" Y="120.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="300.0000" Y="337.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5000" Y="0.5185" />
                    <PreSize X="0.8000" Y="0.1846" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="desc_2_1" ActionTag="221288512" VisibleForFrame="False" Tag="47" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="78.0000" RightMargin="78.0000" TopMargin="300.0000" BottomMargin="250.0000" IsCustomSize="True" FontSize="24" LabelText="111" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Bottom" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="444.0000" Y="100.0000" />
                    <AnchorPoint ScaleX="0.5000" />
                    <Position X="300.0000" Y="250.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5000" Y="0.3846" />
                    <PreSize X="0.7400" Y="0.1538" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="desc_2_2" ActionTag="-1862478856" VisibleForFrame="False" Tag="46" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="277.5000" RightMargin="277.5000" TopMargin="385.5000" BottomMargin="235.5000" FontSize="24" LabelText="222" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="45.0000" Y="29.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="300.0000" Y="250.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="39" G="175" B="229" />
                    <PrePosition X="0.5000" Y="0.3846" />
                    <PreSize X="0.0750" Y="0.0446" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="desc_2_3" ActionTag="-1474953054" VisibleForFrame="False" Tag="49" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="78.0000" RightMargin="78.0000" TopMargin="400.0000" BottomMargin="150.0000" IsCustomSize="True" FontSize="24" LabelText="333" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="444.0000" Y="100.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                    <Position X="300.0000" Y="250.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5000" Y="0.3846" />
                    <PreSize X="0.7400" Y="0.1538" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="email_bg" ActionTag="246548932" Tag="12" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="80.0000" RightMargin="80.0000" TopMargin="369.5000" BottomMargin="219.5000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="18" Scale9Height="31" ctype="ImageViewObjectData">
                    <Size X="440.0000" Y="61.0000" />
                    <Children>
                      <AbstractNodeData Name="ScrollView_2" ActionTag="-2097946430" Tag="358" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="15.4400" RightMargin="14.5600" TopMargin="6.0000" BottomMargin="5.0000" TouchEnable="True" ClipAble="True" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" IsBounceEnabled="True" ScrollDirectionType="Horizontal" ctype="ScrollViewObjectData">
                        <Size X="410.0000" Y="50.0000" />
                        <Children>
                          <AbstractNodeData Name="email_textfield" ActionTag="-1246986149" Tag="13" IconVisible="False" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="1.7700" RightMargin="-1231.7700" TopMargin="8.1000" BottomMargin="259.9000" TouchEnable="True" FontSize="22" IsCustomSize="True" LabelText="" PlaceHolderText="Enter your email address here" MaxLengthEnable="True" MaxLengthText="100" ctype="TextFieldObjectData">
                            <Size X="1640.0000" Y="32.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position X="1.7700" Y="275.9000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="39" G="175" B="229" />
                            <PrePosition X="0.0043" Y="0.9197" />
                            <PreSize X="4.0000" Y="0.1067" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="220.4400" Y="30.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5010" Y="0.4918" />
                        <PreSize X="0.9318" Y="0.8197" />
                        <SingleColor A="255" R="255" G="255" B="255" />
                        <FirstColor A="255" R="255" G="150" B="100" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                        <InnerNodeSize Width="410" Height="300" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="300.0000" Y="250.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.3846" />
                    <PreSize X="0.7333" Y="0.0938" />
                    <FileData Type="Normal" Path="mjstory/material/mat_bg_email.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="warning_icon" ActionTag="-897079170" Tag="14" IconVisible="False" LeftMargin="92.3963" RightMargin="478.6037" TopMargin="440.6517" BottomMargin="184.3483" LeftEage="9" RightEage="9" TopEage="8" BottomEage="8" Scale9OriginX="9" Scale9OriginY="8" Scale9Width="11" Scale9Height="9" ctype="ImageViewObjectData">
                    <Size X="29.0000" Y="25.0000" />
                    <Children>
                      <AbstractNodeData Name="warning_text" ActionTag="1055121095" Tag="15" IconVisible="False" LeftMargin="36.0000" RightMargin="-427.0000" TopMargin="4.0000" BottomMargin="-79.0000" IsCustomSize="True" FontSize="18" LabelText="there is a problem!" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="420.0000" Y="100.0000" />
                        <AnchorPoint ScaleY="1.0000" />
                        <Position X="36.0000" Y="21.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="51" B="0" />
                        <PrePosition X="1.2414" Y="0.8400" />
                        <PreSize X="14.4828" Y="4.0000" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="106.8963" Y="196.8483" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1782" Y="0.3028" />
                    <PreSize X="0.0483" Y="0.0385" />
                    <FileData Type="Normal" Path="mjstory/material/mat_icon_warning.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btedit" ActionTag="-609774123" VisibleForFrame="False" CallBackType="Click" CallBackName="onEditEmail" Tag="33" IconVisible="False" LeftMargin="465.0000" RightMargin="83.0000" TopMargin="375.0000" BottomMargin="225.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="-15" Scale9OriginY="-11" Scale9Width="30" Scale9Height="22" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="52.0000" Y="50.0000" />
                    <Children>
                      <AbstractNodeData Name="Panel_2" ActionTag="301212376" Tag="55" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="52.0000" Y="50.0000" />
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
                      <AbstractNodeData Name="Image_1" ActionTag="2097740530" Tag="34" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="11.0000" RightMargin="11.0000" TopMargin="7.5000" BottomMargin="7.5000" LeftEage="9" RightEage="9" TopEage="11" BottomEage="11" Scale9OriginX="9" Scale9OriginY="11" Scale9Width="12" Scale9Height="13" ctype="ImageViewObjectData">
                        <Size X="30.0000" Y="35.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="26.0000" Y="25.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.5769" Y="0.7000" />
                        <FileData Type="Normal" Path="mjstory/material/mat_icon_edit_email.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="491.0000" Y="250.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8183" Y="0.3846" />
                    <PreSize X="0.0867" Y="0.0769" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="popup_note" ActionTag="1414154074" VisibleForFrame="False" Tag="126" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="50.0000" RightMargin="50.0000" TopMargin="220.0000" BottomMargin="130.0000" TouchEnable="True" ClipAble="False" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="500.0000" Y="300.0000" />
                    <Children>
                      <AbstractNodeData Name="title" ActionTag="1499617145" Tag="127" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="188.0000" RightMargin="188.0000" TopMargin="47.5000" BottomMargin="223.5000" FontSize="24" LabelText="Chú thích" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="124.0000" Y="29.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="250.0000" Y="238.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.7933" />
                        <PreSize X="0.2480" Y="0.0967" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Image_2" ActionTag="447344867" Tag="133" IconVisible="False" LeftMargin="16.0000" RightMargin="460.0000" TopMargin="123.0000" BottomMargin="153.0000" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="10" Scale9Height="10" ctype="ImageViewObjectData">
                        <Size X="24.0000" Y="24.0000" />
                        <Children>
                          <AbstractNodeData Name="txt_down_story" ActionTag="1191364256" Tag="128" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="25.0000" RightMargin="-149.0000" FontSize="20" LabelText="Truyện đã đọc" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="148.0000" Y="24.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position X="25.0000" Y="12.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition X="1.0417" Y="0.5000" />
                            <PreSize X="6.1667" Y="1.0000" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="28.0000" Y="165.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.0560" Y="0.5500" />
                        <PreSize X="0.0480" Y="0.0800" />
                        <FileData Type="Normal" Path="mjstory/material/mat_icon_tick.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="txt_down_ws" ActionTag="1835068579" Tag="130" IconVisible="False" LeftMargin="334.0000" RightMargin="19.0000" TopMargin="123.0000" BottomMargin="153.0000" FontSize="20" LabelText="Học liệu đã tải" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="147.0000" Y="24.0000" />
                        <Children>
                          <AbstractNodeData Name="Image_3" ActionTag="-952296400" Tag="134" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="-30.0000" RightMargin="153.0000" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="10" Scale9Height="10" ctype="ImageViewObjectData">
                            <Size X="24.0000" Y="24.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="-18.0000" Y="12.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="-0.1224" Y="0.5000" />
                            <PreSize X="0.1633" Y="1.0000" />
                            <FileData Type="Normal" Path="mjstory/material/mat_icon_downloaded.png" Plist="" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                        <Position X="481.0000" Y="165.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.9620" Y="0.5500" />
                        <PreSize X="0.2940" Y="0.0800" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="txt_desc" ActionTag="-1838548477" Tag="132" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="40.0000" RightMargin="40.0000" TopMargin="178.0000" BottomMargin="42.0000" IsCustomSize="True" FontSize="20" LabelText="Ba mẹ ưu tiên tải và in học liệu cho các truyện bé đã đọc nhé!" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="420.0000" Y="80.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="250.0000" Y="82.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.2733" />
                        <PreSize X="0.8400" Y="0.2667" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="300.0000" Y="280.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.4308" />
                    <PreSize X="0.8333" Y="0.4615" />
                    <SingleColor A="255" R="255" G="255" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="popup_free_limit" ActionTag="1637843435" VisibleForFrame="False" Tag="57" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="50.0000" RightMargin="50.0000" TopMargin="220.0000" BottomMargin="130.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="500.0000" Y="300.0000" />
                    <Children>
                      <AbstractNodeData Name="desc" ActionTag="-1666532465" Tag="58" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="15.0000" RightMargin="15.0000" TopMargin="142.5000" BottomMargin="32.5000" IsCustomSize="True" FontSize="24" LabelText="Tài khoản miễn phí cho phép tải 5 học liệu/ngày. Vui lòng đăng ký mua để sử dụng không giới hạn!" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="470.0000" Y="125.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="250.0000" Y="95.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.3167" />
                        <PreSize X="0.9400" Y="0.4167" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Image_4" ActionTag="-1837640457" Tag="134" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="196.5000" RightMargin="196.5000" TopMargin="43.0000" BottomMargin="163.0000" LeftEage="35" RightEage="35" TopEage="31" BottomEage="31" Scale9OriginX="35" Scale9OriginY="31" Scale9Width="37" Scale9Height="32" ctype="ImageViewObjectData">
                        <Size X="107.0000" Y="94.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="250.0000" Y="210.0000" />
                        <Scale ScaleX="0.7500" ScaleY="0.7500" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.7000" />
                        <PreSize X="0.2140" Y="0.3133" />
                        <FileData Type="Normal" Path="mjstory/material/mat_icon_warning_big.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="300.0000" Y="280.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.4308" />
                    <PreSize X="0.8333" Y="0.4615" />
                    <SingleColor A="255" R="255" G="255" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btok" ActionTag="1445712659" CallBackType="Click" CallBackName="onOK" Tag="7" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="39.2000" RightMargin="36.8000" TopMargin="518.9599" BottomMargin="41.0401" TouchEnable="True" FontSize="35" ButtonText="Save" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="494" Scale9Height="68" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="524.0000" Y="90.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="301.2000" Y="86.0401" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5020" Y="0.1324" />
                    <PreSize X="0.8733" Y="0.1385" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                    <TextColor A="255" R="255" G="255" B="255" />
                    <DisabledFileData Type="Normal" Path="mjstory/popup/popup_btok.png" Plist="" />
                    <PressedFileData Type="Normal" Path="mjstory/popup/popup_btok.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/popup/popup_btok.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="201.5000" Y="248.9888" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6256" />
                <PreSize X="1.4888" Y="1.6332" />
                <FileData Type="Normal" Path="mjstory/popup/popup_bg_new.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="btcancel" ActionTag="794197901" CallBackType="Click" CallBackName="onCancel" Tag="35" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="TopEdge" LeftMargin="438.5000" RightMargin="-82.5000" TopMargin="22.5000" BottomMargin="328.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="17" Scale9Height="25" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="47.0000" Y="47.0000" />
                <Children>
                  <AbstractNodeData Name="Panel_3" Visible="False" ActionTag="-65962201" CallBackType="Click" CallBackName="onCancel" Tag="599" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="-9.4000" RightMargin="-9.4000" TopMargin="-9.4000" BottomMargin="-9.4000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="65.8000" Y="65.8000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="23.5000" Y="23.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.4000" Y="1.4000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="label_no" ActionTag="-1447711266" VisibleForFrame="False" Tag="36" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="23.5000" RightMargin="23.5000" TopMargin="28.0543" BottomMargin="18.9457" FontSize="25" LabelText="" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="0.0000" Y="0.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="23.5000" Y="18.9457" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.4031" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FontResource Type="Normal" Path="fonts/Linotte Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="462.0000" Y="352.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="1.1464" Y="0.8844" />
                <PreSize X="0.1166" Y="0.1181" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/popup/popup_ratebuy_btclose.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="329.0700" />
            <Scale ScaleX="0.0100" ScaleY="0.0100" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.4285" />
            <PreSize X="0.3936" Y="0.5182" />
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