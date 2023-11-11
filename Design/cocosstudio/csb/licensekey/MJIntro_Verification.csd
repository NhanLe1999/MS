<GameFile>
  <PropertyGroup Name="MJIntro_Verification" Type="Layer" ID="f42bf9ba-584d-40f2-b6fd-81fd39621531" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="80" Speed="1.0000" ActivedAnimationName="wrong">
        <Timeline ActionTag="-397674253" Property="Position">
          <PointFrame FrameIndex="0" X="300.0000" Y="200.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="10" X="320.0000" Y="210.0000">
            <EasingData Type="23" />
          </PointFrame>
          <PointFrame FrameIndex="60" X="300.0000" Y="210.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-397674253" Property="CColor">
          <ColorFrame FrameIndex="0" Alpha="255">
            <EasingData Type="0" />
            <Color A="255" R="77" G="77" B="77" />
          </ColorFrame>
          <ColorFrame FrameIndex="10" Alpha="255">
            <EasingData Type="0" />
            <Color A="255" R="244" G="88" B="88" />
          </ColorFrame>
          <ColorFrame FrameIndex="50" Alpha="255">
            <EasingData Type="0" />
            <Color A="255" R="244" G="88" B="88" />
          </ColorFrame>
          <ColorFrame FrameIndex="60" Alpha="255">
            <EasingData Type="0" />
            <Color A="255" R="77" G="77" B="77" />
          </ColorFrame>
          <ColorFrame FrameIndex="65" Alpha="255">
            <EasingData Type="0" />
            <Color A="255" R="77" G="77" B="77" />
          </ColorFrame>
          <ColorFrame FrameIndex="80" Alpha="255">
            <EasingData Type="0" />
            <Color A="255" R="0" G="156" B="0" />
          </ColorFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="wrong" StartIndex="0" EndIndex="60">
          <RenderColor A="255" R="123" G="104" B="238" />
        </AnimationInfo>
        <AnimationInfo Name="right" StartIndex="65" EndIndex="80">
          <RenderColor A="255" R="255" G="248" B="220" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="MJIntroVerification" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="Panel_1" ActionTag="-645618540" Tag="2" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" ColorAngle="90.0000" LeftEage="675" RightEage="675" TopEage="378" BottomEage="378" Scale9OriginX="-675" Scale9OriginY="-378" Scale9Width="1350" Scale9Height="756" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="content" ActionTag="-1991700070" Tag="4" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="169.5000" RightMargin="169.5000" TopMargin="186.4000" BottomMargin="109.6000" LeftEage="12" RightEage="12" TopEage="12" BottomEage="12" Scale9OriginX="12" Scale9OriginY="12" Scale9Width="650" Scale9Height="437" ctype="ImageViewObjectData">
                <Size X="685.0000" Y="472.0000" />
                <Children>
                  <AbstractNodeData Name="Image_44" ActionTag="-1512992113" Tag="148" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-7.0000" RightMargin="-7.0000" TopMargin="-231.5000" BottomMargin="240.5000" LeftEage="93" RightEage="93" TopEage="52" BottomEage="52" Scale9OriginX="93" Scale9OriginY="52" Scale9Width="513" Scale9Height="359" ctype="ImageViewObjectData">
                    <Size X="699.0000" Y="463.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="342.5000" Y="472.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="1.0000" />
                    <PreSize X="1.0204" Y="0.9809" />
                    <FileData Type="Normal" Path="mjstory/monkey_logo_vi.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_detail" ActionTag="2055778402" Tag="197" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="132.0000" RightMargin="132.0000" TopMargin="92.9998" BottomMargin="315.0002" FontSize="28" LabelText="Chúng tôi đã gửi mã số xác nhận &#xA;vào số điện thoại " HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="421.0000" Y="64.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="342.5000" Y="347.0002" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5000" Y="0.7352" />
                    <PreSize X="0.6146" Y="0.1356" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Panel_2" ActionTag="-397674253" Tag="226" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="117.5000" RightMargin="117.5000" TopMargin="222.0000" BottomMargin="150.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="450.0000" Y="100.0000" />
                    <Children>
                      <AbstractNodeData Name="Panel_3" ActionTag="1212185429" Tag="231" IconVisible="False" RightMargin="360.0000" TopMargin="96.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="90.0000" Y="4.0000" />
                        <Children>
                          <AbstractNodeData Name="number_0" ActionTag="-1101254790" Tag="235" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="17.0000" RightMargin="17.0000" TopMargin="-80.5000" BottomMargin="-28.5000" FontSize="100" LabelText="9" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="56.0000" Y="113.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="45.0000" Y="28.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="7.0000" />
                            <PreSize X="0.6222" Y="28.2500" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint />
                        <Position />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition />
                        <PreSize X="0.2000" Y="0.0400" />
                        <SingleColor A="255" R="255" G="255" B="255" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Panel_3_0" ActionTag="1113415541" Tag="232" IconVisible="False" LeftMargin="120.0000" RightMargin="240.0000" TopMargin="96.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="90.0000" Y="4.0000" />
                        <Children>
                          <AbstractNodeData Name="number_1" ActionTag="770564696" Tag="236" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="17.0000" RightMargin="17.0000" TopMargin="-80.5000" BottomMargin="-28.5000" FontSize="100" LabelText="9" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="56.0000" Y="113.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="45.0000" Y="28.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="7.0000" />
                            <PreSize X="0.6222" Y="28.2500" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" />
                        <Position X="165.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.3667" />
                        <PreSize X="0.2000" Y="0.0400" />
                        <SingleColor A="255" R="255" G="255" B="255" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Panel_3_1" ActionTag="-2056296511" Tag="233" IconVisible="False" LeftMargin="240.0000" RightMargin="120.0000" TopMargin="96.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="90.0000" Y="4.0000" />
                        <Children>
                          <AbstractNodeData Name="number_2" ActionTag="1462331907" Tag="237" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="17.0000" RightMargin="17.0000" TopMargin="-80.5000" BottomMargin="-28.5000" FontSize="100" LabelText="9" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="56.0000" Y="113.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="45.0000" Y="28.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="7.0000" />
                            <PreSize X="0.6222" Y="28.2500" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint />
                        <Position X="240.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5333" />
                        <PreSize X="0.2000" Y="0.0400" />
                        <SingleColor A="255" R="255" G="255" B="255" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Panel_3_2" ActionTag="1061513399" Tag="234" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="BottomEdge" LeftMargin="360.0000" TopMargin="96.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="90.0000" Y="4.0000" />
                        <Children>
                          <AbstractNodeData Name="number_3" ActionTag="-1255236760" Tag="238" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="17.0000" RightMargin="17.0000" TopMargin="-80.5000" BottomMargin="-28.5000" FontSize="100" LabelText="9" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="56.0000" Y="113.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="45.0000" Y="28.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="7.0000" />
                            <PreSize X="0.6222" Y="28.2500" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint />
                        <Position X="360.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.8000" />
                        <PreSize X="0.2000" Y="0.0400" />
                        <SingleColor A="255" R="255" G="255" B="255" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="button_trigger" ActionTag="1500195181" CallBackType="Click" CallBackName="onEnterCode" Tag="126" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="-15" Scale9OriginY="-6" Scale9Width="30" Scale9Height="12" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="450.0000" Y="100.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="225.0000" Y="50.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.0000" Y="1.0000" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="342.5000" Y="200.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="77" G="77" B="77" />
                    <PrePosition X="0.5000" Y="0.4237" />
                    <PreSize X="0.6569" Y="0.2119" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_2" ActionTag="-2134063901" Tag="227" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="319.5000" RightMargin="319.5000" TopMargin="399.0000" BottomMargin="27.0000" Scale9Enable="True" LeftEage="12" RightEage="12" TopEage="12" BottomEage="12" Scale9OriginX="12" Scale9OriginY="12" Scale9Width="22" Scale9Height="22" ctype="ImageViewObjectData">
                    <Size X="46.0000" Y="46.0000" />
                    <Children>
                      <AbstractNodeData Name="resend_progress" ActionTag="-1952879638" Tag="229" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-152.0000" RightMargin="-152.0000" TopMargin="-2.0000" BottomMargin="-2.0000" ProgressInfo="34" ctype="LoadingBarObjectData">
                        <Size X="350.0000" Y="50.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="23.0000" Y="23.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="7.6087" Y="1.0870" />
                        <ImageFileData Type="Normal" Path="license/active_license/activelicense_btsend.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="resend_label" ActionTag="916659682" Tag="230" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-126.0000" RightMargin="-126.0000" TopMargin="11.5000" BottomMargin="11.5000" FontSize="20" LabelText="Yêu cầu mã số mới sau 0:30 giây" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="298.0000" Y="23.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="23.0000" Y="23.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="6.4783" Y="0.5000" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="button_resend" ActionTag="1514991532" CallBackType="Click" CallBackName="onResendCode" Tag="240" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="15" Scale9OriginY="6" Scale9Width="16" Scale9Height="24" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="46.0000" Y="46.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="23.0000" Y="23.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.0000" Y="1.0000" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="342.5000" Y="50.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.1059" />
                    <PreSize X="0.0672" Y="0.0975" />
                    <FileData Type="Default" Path="Default/ImageFile.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Text_1_0" ActionTag="2006680894" Tag="198" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="255.0000" RightMargin="255.0000" TopMargin="370.0001" BottomMargin="80.9999" FontSize="15" LabelText="Không nhận được mã số?" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="175.0000" Y="21.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="342.5000" Y="91.4999" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="84" G="84" B="84" />
                    <PrePosition X="0.5000" Y="0.1939" />
                    <PreSize X="0.2555" Y="0.0445" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Text_1_0_0" ActionTag="-818600710" Tag="225" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="219.0000" RightMargin="219.0000" TopMargin="177.3000" BottomMargin="271.7000" FontSize="20" LabelText="Vui lòng nhập mã số tại đây" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="247.0000" Y="23.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="342.5000" Y="283.2000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="84" G="84" B="84" />
                    <PrePosition X="0.5000" Y="0.6000" />
                    <PreSize X="0.3606" Y="0.0487" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="345.6000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4500" />
                <PreSize X="0.6689" Y="0.6146" />
                <FileData Type="Normal" Path="license/active_license/activelicense_bg.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="185" G="216" B="42" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="Button_2" ActionTag="-1264825304" CallBackType="Click" CallBackName="onBack" Tag="114" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="25.0000" RightMargin="874.0000" TopMargin="25.0000" BottomMargin="703.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="8" RightEage="8" TopEage="11" BottomEage="11" Scale9OriginX="-8" Scale9OriginY="-11" Scale9Width="16" Scale9Height="22" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="125.0000" Y="40.0000" />
            <Children>
              <AbstractNodeData Name="Image_1" ActionTag="1202885285" Tag="180" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="1.3319" RightMargin="83.6681" TopMargin="2.7714" BottomMargin="-2.7714" LeftEage="8" RightEage="8" TopEage="8" BottomEage="8" Scale9OriginX="8" Scale9OriginY="8" Scale9Width="24" Scale9Height="24" ctype="ImageViewObjectData">
                <Size X="40.0000" Y="40.0000" />
                <AnchorPoint ScaleX="0.2445" ScaleY="0.6848" />
                <Position X="11.1135" Y="24.6190" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0889" Y="0.6155" />
                <PreSize X="0.3200" Y="1.0000" />
                <FileData Type="Normal" Path="mjstory/new_ui_vn/close_btn.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Text_10" ActionTag="1780744853" Tag="115" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="45.5000" RightMargin="5.5000" TopMargin="9.5000" BottomMargin="7.5000" FontSize="20" LabelText="Quay lại" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="74.0000" Y="23.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="82.5000" Y="19.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.6600" Y="0.4750" />
                <PreSize X="0.5920" Y="0.5750" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="87.5000" Y="723.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.0854" Y="0.9414" />
            <PreSize X="0.1221" Y="0.0521" />
            <TextColor A="255" R="65" G="65" B="70" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="button_skip" ActionTag="-737751266" CallBackType="Click" CallBackName="onSkip" Tag="124" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="BottomEdge" LeftMargin="474.0000" TopMargin="728.0000" BottomMargin="10.0000" TouchEnable="True" FontSize="18" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="-15" Scale9OriginY="-6" Scale9Width="30" Scale9Height="12" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="550.0000" Y="30.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="749.0000" Y="25.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.7314" Y="0.0326" />
            <PreSize X="0.5371" Y="0.0391" />
            <TextColor A="255" R="255" G="255" B="255" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="text_skip" ActionTag="2018928474" Tag="125" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="BottomEdge" LeftMargin="478.0000" RightMargin="20.0000" TopMargin="730.5000" BottomMargin="16.5000" FontSize="18" LabelText="• Nếu bạn không nhận được mã xác nhận, vui lòng bấm vào đây •" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="526.0000" Y="21.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="741.0000" Y="27.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.7236" Y="0.0352" />
            <PreSize X="0.5137" Y="0.0273" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>