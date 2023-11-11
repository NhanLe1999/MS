<GameFile>
  <PropertyGroup Name="RatingReviewPopup" Type="Layer" ID="0acdda22-2079-47fb-a5ab-c7ef033a9665" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="RatingReviewPopup" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="popup_bg" ActionTag="-849190111" Tag="103" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="78" Scale9Height="37" ctype="ImageViewObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="rating/rating_bg.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="title_bg" ActionTag="-1044866128" Tag="116" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TopMargin="-3.4944" BottomMargin="680.4863" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="6" Scale9OriginY="15" Scale9Width="9" Scale9Height="62" ctype="ImageViewObjectData">
            <Size X="1024.0000" Y="91.0080" />
            <Children>
              <AbstractNodeData Name="exit_button" ActionTag="-295452577" CallBackType="Click" CallBackName="onCancel" Tag="314" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="2.7680" RightMargin="961.2320" TopMargin="7.5044" BottomMargin="23.5036" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="60.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="1291215994" Tag="315" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="20.0000" RightMargin="20.0000" TopMargin="14.0000" BottomMargin="14.0000" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="6" Scale9Height="18" ctype="ImageViewObjectData">
                    <Size X="20.0000" Y="32.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="30.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.3333" Y="0.5333" />
                    <FileData Type="Normal" Path="rating/exit.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="32.7680" Y="53.5036" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0320" Y="0.5879" />
                <PreSize X="0.0586" Y="0.6593" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="725.9904" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.9453" />
            <PreSize X="1.0000" Y="0.1185" />
            <FileData Type="Normal" Path="rating/title_bg.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="title" ActionTag="-729492113" Tag="177" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="237.2592" RightMargin="240.7408" TopMargin="107.1680" BottomMargin="604.8320" FontSize="48" LabelText="Help us improve, please?" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="546.0000" Y="56.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="510.2592" Y="632.8320" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.4983" Y="0.8240" />
            <PreSize X="0.5332" Y="0.0729" />
            <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="description" ActionTag="113822167" Tag="178" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="55.7944" RightMargin="39.2056" TopMargin="175.4352" BottomMargin="526.5648" FontSize="28" LabelText="Nếu như bạn chưa biết cách sử dụng Monkey Junior, hãy truy cập trang của &#xA;chúng tôi ĐỒNG HÀNH CÙNG CON HỌC MONKEY hoặc viết một đánh giá?" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="929.0000" Y="66.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="520.2944" Y="559.5648" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.5081" Y="0.7286" />
            <PreSize X="0.9072" Y="0.0859" />
            <FontResource Type="Normal" Path="fonts/Roboto-Regular.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="review_textbox" Visible="False" ActionTag="-302268375" Tag="267" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="97.0000" RightMargin="97.0000" TopMargin="397.2900" BottomMargin="190.7100" TouchEnable="True" FontSize="25" IsCustomSize="True" LabelText="" PlaceHolderText="Write your review" MaxLengthText="10" ctype="TextFieldObjectData">
            <Size X="830.0000" Y="180.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="280.7100" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.5000" Y="0.3655" />
            <PreSize X="0.8105" Y="0.2344" />
            <FontResource Type="Normal" Path="fonts/Roboto-Regular.ttf" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="post_button" ActionTag="2129361811" CallBackType="Click" CallBackName="onPost" Tag="181" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="293.5000" RightMargin="293.5000" TopMargin="641.7419" BottomMargin="51.2581" TouchEnable="True" FontSize="14" ButtonText="Button" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="15" Scale9OriginY="5" Scale9Width="10" Scale9Height="26" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="437.0000" Y="75.0000" />
            <Children>
              <AbstractNodeData Name="post_txt" ActionTag="-1853821742" Tag="183" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="105.5000" RightMargin="105.5000" TopMargin="20.0000" BottomMargin="20.0000" FontSize="30" LabelText="Post your review" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="226.0000" Y="35.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="218.5000" Y="37.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.5172" Y="0.4667" />
                <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="88.7581" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.1156" />
            <PreSize X="0.4268" Y="0.0977" />
            <TextColor A="255" R="65" G="65" B="70" />
            <NormalFileData Type="Normal" Path="rating/button_bg.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="textbox_bg" ActionTag="-1831860031" Tag="180" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="86.5000" RightMargin="86.5000" TopMargin="387.2888" BottomMargin="180.7112" Scale9Enable="True" LeftEage="13" RightEage="13" TopEage="7" BottomEage="7" Scale9OriginX="13" Scale9OriginY="7" Scale9Width="14" Scale9Height="8" ctype="ImageViewObjectData">
            <Size X="851.0000" Y="200.0000" />
            <Children>
              <AbstractNodeData Name="combo_box" ActionTag="-2119103371" Tag="182" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TopMargin="-64.1600" BottomMargin="211.1600" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="15" Scale9OriginY="5" Scale9Width="10" Scale9Height="26" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="851.0000" Y="53.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="425.5000" Y="237.6600" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="1.1883" />
                <PreSize X="1.0000" Y="0.2650" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="rating/button_bg.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="facebook_button" ActionTag="-1617270969" CallBackType="Click" CallBackName="onFacebookClicked" Tag="211" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="286.5000" RightMargin="286.5000" TopMargin="-129.7591" BottomMargin="276.7591" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="15" Scale9OriginY="5" Scale9Width="10" Scale9Height="26" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="278.0000" Y="53.0000" />
                <Children>
                  <AbstractNodeData Name="fb_txt" ActionTag="1069359446" Tag="224" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="76.1228" RightMargin="46.8772" TopMargin="13.2773" BottomMargin="10.7227" FontSize="25" LabelText="Visit our page" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="155.0000" Y="29.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="153.6228" Y="25.2227" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5526" Y="0.4759" />
                    <PreSize X="0.5576" Y="0.5472" />
                    <FontResource Type="Normal" Path="fonts/Roboto-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="fb_icon" ActionTag="-933364040" Tag="212" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="20.0244" RightMargin="236.9756" TopMargin="6.9594" BottomMargin="8.0406" ctype="SpriteObjectData">
                    <Size X="21.0000" Y="38.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="30.5244" Y="27.0406" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1098" Y="0.5102" />
                    <PreSize X="0.0755" Y="0.7170" />
                    <FileData Type="Normal" Path="rating/fb_icon.png" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="425.5000" Y="303.2591" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="1.5163" />
                <PreSize X="0.3267" Y="0.2650" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="rating/button_bg.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="280.7112" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.3655" />
            <PreSize X="0.8311" Y="0.2604" />
            <FileData Type="Normal" Path="rating/textbox_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>