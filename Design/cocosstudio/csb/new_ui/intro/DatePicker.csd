<GameFile>
  <PropertyGroup Name="DatePicker" Type="Layer" ID="4aeb555f-9315-433b-8214-03b872113baa" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="70" Speed="1.0000" ActivedAnimationName="fade_in">
        <Timeline ActionTag="1612405095" Property="Scale">
          <ScaleFrame FrameIndex="5" X="0.0100" Y="0.0100">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="1.0000" Y="1.0000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="40" X="1.0000" Y="1.0000">
            <EasingData Type="25" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="70" X="0.0100" Y="0.0100">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="1612405095" Property="RotationSkew">
          <ScaleFrame FrameIndex="5" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="0.0000" Y="0.0000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="40" X="0.0000" Y="0.0000">
            <EasingData Type="25" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="70" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="fade_in" StartIndex="5" EndIndex="35">
          <RenderColor A="255" R="255" G="99" B="71" />
        </AnimationInfo>
        <AnimationInfo Name="fade_out" StartIndex="40" EndIndex="75">
          <RenderColor A="255" R="240" G="248" B="255" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="DatePicker" Tag="1039" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="Panel_1" ActionTag="675623581" Tag="1040" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="126" ComboBoxIndex="1" ColorAngle="90.0000" LeftEage="198" RightEage="198" TopEage="276" BottomEage="276" Scale9OriginX="-198" Scale9OriginY="-276" Scale9Width="396" Scale9Height="552" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="popup_panel" ActionTag="1612405095" Tag="1041" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="256.0001" RightMargin="255.9999" TopMargin="95.2000" BottomMargin="248.8000" TouchEnable="True" ClipAble="False" ColorAngle="90.0000" LeftEage="206" RightEage="206" TopEage="224" BottomEage="224" Scale9OriginX="206" Scale9OriginY="224" Scale9Width="188" Scale9Height="390" ctype="PanelObjectData">
                <Size X="512.0000" Y="424.0000" />
                <Children>
                  <AbstractNodeData Name="title" ActionTag="-713143894" Tag="1046" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="31.0000" RightMargin="31.0000" TopMargin="66.9392" BottomMargin="257.0608" IsCustomSize="True" FontSize="20" LabelText="Mời bạn nhập ngày sinh nhật của con&#xA;để VMonkey cung cấp nội dung và &#xA;các chương trình chăm sóc, ưu đãi phù hợp" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="450.0000" Y="100.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="256.0000" Y="307.0608" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5000" Y="0.7242" />
                    <PreSize X="0.8789" Y="0.2358" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Panel_2" ActionTag="533733651" Tag="147" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="9.4760" RightMargin="11.5240" TopMargin="224.0208" BottomMargin="-0.0208" TouchEnable="True" ClipAble="False" BackColorAlpha="25" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="491.0000" Y="200.0000" />
                    <Children>
                      <AbstractNodeData Name="Date" ActionTag="-1717491430" Tag="1049" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="76.1050" RightMargin="360.8850" TopMargin="10.0000" BottomMargin="10.0000" TouchEnable="True" ClipAble="True" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" IsBounceEnabled="True" ScrollDirectionType="0" DirectionType="Vertical" HorizontalType="Align_HorizontalCenter" ctype="ListViewObjectData">
                        <Size X="54.0100" Y="180.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="103.1100" Y="100.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.2100" Y="0.5000" />
                        <PreSize X="0.1100" Y="0.9000" />
                        <SingleColor A="255" R="150" G="150" B="255" />
                        <FirstColor A="255" R="150" G="150" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Month" ActionTag="-1338352269" Tag="1050" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="147.3000" RightMargin="176.7600" TopMargin="10.0000" BottomMargin="10.0000" TouchEnable="True" ClipAble="True" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" IsBounceEnabled="True" ScrollDirectionType="0" DirectionType="Vertical" HorizontalType="Align_HorizontalCenter" ctype="ListViewObjectData">
                        <Size X="166.9400" Y="180.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="230.7700" Y="100.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.4700" Y="0.5000" />
                        <PreSize X="0.3400" Y="0.9000" />
                        <SingleColor A="255" R="150" G="150" B="255" />
                        <FirstColor A="255" R="150" G="150" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Year" ActionTag="-422468887" Tag="1051" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="331.4250" RightMargin="66.2850" TopMargin="10.0000" BottomMargin="10.0000" TouchEnable="True" ClipAble="True" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" IsBounceEnabled="True" ScrollDirectionType="0" DirectionType="Vertical" HorizontalType="Align_HorizontalCenter" ctype="ListViewObjectData">
                        <Size X="93.2900" Y="180.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="378.0700" Y="100.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.7700" Y="0.5000" />
                        <PreSize X="0.1900" Y="0.9000" />
                        <SingleColor A="255" R="150" G="150" B="255" />
                        <FirstColor A="255" R="150" G="150" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="254.9760" Y="99.9792" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4980" Y="0.2358" />
                    <PreSize X="0.9590" Y="0.4717" />
                    <SingleColor A="255" R="0" G="0" B="0" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btn" ActionTag="-1139644757" CallBackType="Click" Tag="1047" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="156.5000" RightMargin="156.5000" TopMargin="486.0080" BottomMargin="-122.0080" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Enable="True" LeftEage="76" RightEage="76" TopEage="16" BottomEage="16" Scale9OriginX="76" Scale9OriginY="16" Scale9Width="80" Scale9Height="18" ctype="PanelObjectData">
                    <Size X="199.0000" Y="60.0000" />
                    <Children>
                      <AbstractNodeData Name="Text_2" ActionTag="-1270824907" Tag="1048" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="26.5000" RightMargin="26.5000" TopMargin="8.5000" BottomMargin="14.5000" FontSize="30" LabelText="Xác nhận" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="146.0000" Y="37.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="99.5000" Y="33.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.5500" />
                        <PreSize X="0.7337" Y="0.6167" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="256.0000" Y="-92.0080" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="-0.2170" />
                    <PreSize X="0.3887" Y="0.1415" />
                    <FileData Type="Normal" Path="mjstory/new_ui_vn/grade_choosen_btn.png" Plist="" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0001" Y="460.8000" />
                <Scale ScaleX="0.0100" ScaleY="0.0100" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6000" />
                <PreSize X="0.5000" Y="0.5521" />
                <FileData Type="Normal" Path="mjstory/new_ui_vn/pop_vn.png" Plist="" />
                <SingleColor A="255" R="128" G="0" B="128" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
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