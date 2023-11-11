<GameFile>
  <PropertyGroup Name="game_noi" Type="Layer" ID="7580fb64-6c6f-4b7b-b309-8257b1760af0" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="120" Speed="1.0000" ActivedAnimationName="appear">
        <Timeline ActionTag="-1384188803" Property="Position">
          <PointFrame FrameIndex="0" X="512.0000" Y="1000.0000">
            <EasingData Type="14" />
          </PointFrame>
          <PointFrame FrameIndex="60" X="512.0000" Y="480.0000">
            <EasingData Type="13" />
          </PointFrame>
          <PointFrame FrameIndex="120" X="512.0000" Y="1400.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-1346849374" Property="Position">
          <PointFrame FrameIndex="0" X="512.0000" Y="-402.0000">
            <EasingData Type="14" />
          </PointFrame>
          <PointFrame FrameIndex="60" X="512.0000" Y="5.0000">
            <EasingData Type="13" />
          </PointFrame>
          <PointFrame FrameIndex="120" X="512.0000" Y="-402.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="appear" StartIndex="0" EndIndex="60">
          <RenderColor A="255" R="255" G="69" B="0" />
        </AnimationInfo>
        <AnimationInfo Name="disappear" StartIndex="60" EndIndex="149">
          <RenderColor A="255" R="106" G="90" B="205" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="GameNoi" Tag="20" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="background" ActionTag="1335363537" Tag="21" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" ColorAngle="90.0000" LeftEage="561" RightEage="561" TopEage="253" BottomEage="253" Scale9OriginX="-561" Scale9OriginY="-253" Scale9Width="1122" Scale9Height="506" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="bg" ActionTag="242787228" Tag="1028" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-341.4816" RightMargin="-334.5184" LeftEage="561" RightEage="561" TopEage="253" BottomEage="253" Scale9OriginX="561" Scale9OriginY="253" Scale9Width="578" Scale9Height="262" ctype="ImageViewObjectData">
                <Size X="1700.0000" Y="768.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="508.5184" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4966" Y="0.5000" />
                <PreSize X="1.6602" Y="1.0000" />
                <FileData Type="Normal" Path="games/vm_game_noi/bg.jpg" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Panel_1" ActionTag="841887789" Tag="70" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="1024.0000" Y="768.0000" />
                <Children>
                  <AbstractNodeData Name="top_frames" ActionTag="-1384188803" Tag="29" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="TopEdge" TopMargin="-367.0000" BottomMargin="865.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="1024.0000" Y="270.0000" />
                    <Children>
                      <AbstractNodeData Name="FileNode_1" ActionTag="-1482234054" Tag="30" IconVisible="True" HorizontalEdge="LeftEdge" LeftMargin="10.0000" RightMargin="784.0000" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
                        <Size X="230.0000" Y="270.0000" />
                        <AnchorPoint />
                        <Position X="10.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.0098" />
                        <PreSize X="0.2246" Y="1.0000" />
                        <FileData Type="Normal" Path="csb/vm_game/vm_game_noi/start_point.csd" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="FileNode_1_0" ActionTag="-19487136" Tag="35" IconVisible="True" HorizontalEdge="LeftEdge" LeftMargin="268.0000" RightMargin="526.0000" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
                        <Size X="230.0000" Y="270.0000" />
                        <AnchorPoint />
                        <Position X="268.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.2617" />
                        <PreSize X="0.2246" Y="1.0000" />
                        <FileData Type="Normal" Path="csb/vm_game/vm_game_noi/start_point.csd" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="FileNode_1_1" ActionTag="-306193998" Tag="40" IconVisible="True" LeftMargin="526.0000" RightMargin="268.0000" TopMargin="0.0001" BottomMargin="-0.0001" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
                        <Size X="230.0000" Y="270.0000" />
                        <AnchorPoint />
                        <Position X="526.0000" Y="-0.0001" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5137" Y="0.0000" />
                        <PreSize X="0.2246" Y="1.0000" />
                        <FileData Type="Normal" Path="csb/vm_game/vm_game_noi/start_point.csd" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="FileNode_1_2" ActionTag="-959072040" Tag="45" IconVisible="True" HorizontalEdge="RightEdge" LeftMargin="784.0000" RightMargin="10.0000" TopMargin="0.0001" BottomMargin="-0.0001" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
                        <Size X="230.0000" Y="270.0000" />
                        <AnchorPoint />
                        <Position X="784.0000" Y="-0.0001" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.7656" Y="0.0000" />
                        <PreSize X="0.2246" Y="1.0000" />
                        <FileData Type="Normal" Path="csb/vm_game/vm_game_noi/start_point.csd" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="512.0000" Y="1000.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="1.3021" />
                    <PreSize X="1.0000" Y="0.3516" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="bottom_frames" ActionTag="-1346849374" Tag="50" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" TopMargin="925.0000" BottomMargin="-402.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="1024.0000" Y="245.0000" />
                    <Children>
                      <AbstractNodeData Name="FileNode_2" ActionTag="65432709" Tag="71" IconVisible="True" HorizontalEdge="LeftEdge" VerticalEdge="BottomEdge" LeftMargin="10.0000" RightMargin="784.0000" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
                        <Size X="230.0000" Y="245.0000" />
                        <AnchorPoint />
                        <Position X="10.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.0098" />
                        <PreSize X="0.2246" Y="1.0000" />
                        <FileData Type="Normal" Path="csb/vm_game/vm_game_noi/end_point.csd" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="FileNode_2_0" ActionTag="739937113" Tag="75" IconVisible="True" HorizontalEdge="RightEdge" VerticalEdge="BottomEdge" LeftMargin="268.0000" RightMargin="526.0000" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
                        <Size X="230.0000" Y="245.0000" />
                        <AnchorPoint />
                        <Position X="268.0000" />
                        <Scale ScaleX="0.9000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.2617" />
                        <PreSize X="0.2246" Y="1.0000" />
                        <FileData Type="Normal" Path="csb/vm_game/vm_game_noi/end_point.csd" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="FileNode_2_1" ActionTag="2019202479" Tag="79" IconVisible="True" HorizontalEdge="RightEdge" VerticalEdge="BottomEdge" LeftMargin="526.0000" RightMargin="268.0000" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
                        <Size X="230.0000" Y="245.0000" />
                        <AnchorPoint />
                        <Position X="526.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5137" />
                        <PreSize X="0.2246" Y="1.0000" />
                        <FileData Type="Normal" Path="csb/vm_game/vm_game_noi/end_point.csd" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="FileNode_2_2" ActionTag="88160860" Tag="83" IconVisible="True" HorizontalEdge="RightEdge" VerticalEdge="BottomEdge" LeftMargin="784.0000" RightMargin="10.0000" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
                        <Size X="230.0000" Y="245.0000" />
                        <AnchorPoint />
                        <Position X="784.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.7656" />
                        <PreSize X="0.2246" Y="1.0000" />
                        <FileData Type="Normal" Path="csb/vm_game/vm_game_noi/end_point.csd" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" />
                    <Position X="512.0000" Y="-402.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="-0.5234" />
                    <PreSize X="1.0000" Y="0.3190" />
                    <SingleColor A="255" R="150" G="200" B="255" />
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
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="panel_question" ActionTag="1065423687" Tag="1029" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" BottomMargin="608.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="1024.0000" Y="160.0000" />
                <Children>
                  <AbstractNodeData Name="bg_ques" ActionTag="-1778087077" Tag="1032" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="174.6766" RightMargin="182.3234" TopMargin="10.0000" BottomMargin="10.0000" LeftEage="220" RightEage="220" TopEage="55" BottomEage="55" Scale9OriginX="220" Scale9OriginY="55" Scale9Width="227" Scale9Height="59" ctype="ImageViewObjectData">
                    <Size X="667.0000" Y="140.0000" />
                    <Children>
                      <AbstractNodeData Name="lbl_question" ActionTag="1019028566" Tag="1033" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="33.5000" RightMargin="33.5000" TopMargin="10.0000" BottomMargin="10.0000" IsCustomSize="True" FontSize="40" LabelText="Dong 1&#xA;Dong 2&#xA;&#xA;" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="600.0000" Y="120.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="333.5000" Y="70.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.8996" Y="0.8571" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="508.1766" Y="80.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4963" Y="0.5000" />
                    <PreSize X="0.6514" Y="0.8750" />
                    <FileData Type="Normal" Path="games/vm_game_noi/bg_ques.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btn_speaker" ActionTag="-1274884344" Tag="1030" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="51.6743" RightMargin="877.3257" TopMargin="32.5000" BottomMargin="32.5000" TouchEnable="True" FontSize="14" ButtonText="Button" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="65" Scale9Height="73" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="95.0000" Y="95.0000" />
                    <Children>
                      <AbstractNodeData Name="Image_403" ActionTag="1284030873" Tag="1031" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="14.0000" RightMargin="14.0000" TopMargin="15.5000" BottomMargin="15.5000" LeftEage="22" RightEage="22" TopEage="21" BottomEage="21" Scale9OriginX="22" Scale9OriginY="21" Scale9Width="23" Scale9Height="22" ctype="ImageViewObjectData">
                        <Size X="67.0000" Y="64.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="47.5000" Y="47.5000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.7053" Y="0.6737" />
                        <FileData Type="Normal" Path="games/vm_game_noi/loa.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="99.1743" Y="80.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0968" Y="0.5000" />
                    <PreSize X="0.0928" Y="0.5938" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Normal" Path="games/vm_game_noi/loabg.png" Plist="" />
                    <NormalFileData Type="Normal" Path="games/vm_game_noi/loabg.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="512.0000" Y="768.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="1.0000" />
                <PreSize X="1.0000" Y="0.2083" />
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
            <FirstColor A="255" R="255" G="250" B="229" />
            <EndColor A="255" R="69" G="241" B="249" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>