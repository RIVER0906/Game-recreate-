
# include <Siv3D.hpp> // OpenSiv3D v0.6.3

struct GameData
{
	int32 score = 0, highscore = 0;
	int32 num = 0;
};
using App = SceneManager<String, GameData>;
class Title :public App::Scene {
public:
	Title(const InitData& init) :IScene(init){
	}
	void update() FMT_OVERRIDE {
		
		if (MouseR.down())
		{
			getData().score = 0;
			changeScene(U"Game");
		}

	}
	void draw() const override
	{
		const Polygon polygon1
		{
			Vec2{ 550, 225 }, Vec2{ 600, 300 }, Vec2{ 550, 375 }, Vec2{ 575, 300 }
		};
		const Polygon polygon2
		{
			//中心(400,300)
			Vec2{ 250, 225 }, Vec2{ 225, 300 }, Vec2{ 250, 375 }, Vec2{ 200, 300 }
		};
		polygon1.draw(Palette::Skyblue);
		polygon2.draw(Palette::Skyblue);
		if (polygon1.mouseOver() )//Right
		{
			polygon1.draw(Palette::White);
			if (MouseL.down())
			{
				getData().num++;

			}
		}
		else
		{
			polygon1.draw(Palette::Skyblue);
		}
		if (polygon2.mouseOver() && getData().num >= 1)//Left
		{
			polygon2.draw(Palette::White);
			if (MouseL.down())
			{
				getData().num--;

			}
		}
		else
		{
			polygon2.draw(Palette::Skyblue);
		}
		// 円座標系における動径座標
		Scene::SetBackground(ColorF(0.3, 0.4, 0.5));
		FontAsset(U"TitleFont")(U"My Game").drawAt(400, 100);
		FontAsset(U"ScoreFont")(U"Score: {}"_fmt(getData().score)).draw(590, 490);
		FontAsset(U"ScoreFont")(U"HighScore: {}"_fmt(getData().highscore)).draw(520, 540);

		//自機
		const Vec2 center = Scene::Center();
		const double t = Scene::Time();
		constexpr double r = 30.0;
		//自キャラ1
		if (getData().num == 0) {
			//自機
			Circle(center, 10).draw(Palette::Orange);
			Rect(center.x - 10, center.y, 20, 1).draw(Palette::Yellow);
			for (auto i : step(5))
			{
				const double theta = i * 72_deg + t * 180_deg;

				const Vec2 pos = OffsetCircular(center, r, theta);

				Circle(pos.x, pos.y, 5).draw(HSV(i * 72 + t * 180));
			}
		}

		//自キャラ2
		if (getData().num == 1) {
			Circle(center.x, center.y, 10).draw(Palette::Orange);
			Rect(center.x- 10, center.y, 20, 1).draw(Palette::Yellow);
			for (auto i : step(2))
			{
				const double theta = 90_deg + i * 180_deg;

				const Vec2 pos = OffsetCircular(center, r, theta);
				Circle(pos.x, pos.y, 7).draw(Palette::Rosybrown);
			}
		}
	}
};
class Game : public App::Scene
{
private:

	Texture m_texture;
	bool gameover = false;
	int32 variable1 = 3, myarg = 0, shotcool1 = 0, shotcool2 = 0, rush = 0, stepcool = 0;
	Array<Vec2> playerBullets;
	Vec2 player1{ 400,500 };
	static constexpr double r = 30.0;
	// 自機のスピード
	static constexpr double playerSpeed = 550.0;
	// 自機ショットのスピード
	static constexpr double playerBulletSpeed = 100.0;
	// 自機ショットのクールタイム（秒）
	static constexpr double playershotcool1Time = 0.5;
	// 自機ショットのクールタイムタイマー
	double playerShotTimer = 0.0;
public:

	Game(const InitData& init)
		: IScene(init)
	{
	}

	void update() override
	{
		
		//GAMEOVER
		if (MouseL.down())
		{
			gameover=true;
		}
		if (gameover == true)
		{
			gameover = false;
			if (getData().score > getData().highscore) {
				getData().highscore = getData().score;
			}
			changeScene(U"Title");
		}
		const double deltaTime = Scene::DeltaTime();
		playerShotTimer = Min(playerShotTimer + deltaTime, playershotcool1Time);
		// 自機の移動
		const Vec2 move = Vec2(KeyRight.pressed() - KeyLeft.pressed(), KeyDown.pressed() - KeyUp.pressed())
			.setLength(deltaTime * playerSpeed * (KeyShift.pressed() ? 2.0 : 1.0));
		player1.moveBy(move).clamp(Scene::Rect());

		// 自機ショットの発射
		if (playerShotTimer >= playershotcool1Time)
		{
			playerShotTimer = 0.0;
			playerBullets << player1.movedBy(0, -50);
		}

		// 自機ショットの移動
		for (auto& playerBullet : playerBullets)
		{
			playerBullet.y += deltaTime * -playerBulletSpeed;
		}
		// 画面外に出た自機ショットは消滅
		playerBullets.remove_if([](const Vec2& b) { return b.y < 140; });

		/*
		if (getData().stepcool <= 0) {
			if (KeyShift.down()) {
				if (KeyLeft.pressed())
				{
					getData().X -= delta*50;
					getData().stepcool = 30;
				}

				if (KeyRight.pressed())
				{
					getData().X += delta*50;
					getData().stepcool = 30;
				}

				if (KeyUp.pressed())
				{
					getData().Y -= delta*50;
					getData().stepcool = 30;
				}

				if (KeyDown.pressed())
				{
					getData().Y += delta*50;
					getData().stepcool = 30;
				}
			}
			else {
				if (getData().shotcool1 <= 0) {
					if (KeyLeft.pressed())
					{
						getData().X -= delta;
					}

					if (KeyRight.pressed())
					{
						getData().X += delta;
					}

					if (KeyUp.pressed())
					{
						getData().Y -= delta;
					}

					if (KeyDown.pressed())
					{
						getData().Y += delta;
					}
				}
			}
		}
		else {
			getData().stepcool--;
		}
		*/
		if (getData().num == 1) {
			if (shotcool1 <= 0) {
				if (KeyX.pressed()) {
					if (myarg <= 44) {
						myarg += 2;
					}
				}
				else if (KeyZ.pressed()) {
					if (myarg < 44) {
						myarg += 4;
					}
					else {
						myarg = 44;
						rush += variable1;
						if (rush > 46 || rush < 0) {
							variable1 *= -1;
						}
					}
				}
				if (KeyX.up()) {
					shotcool1 = myarg * 5;
					myarg *= -1.5;
					rush = 0;
				}
				if (KeyZ.up()) {
					rush = 0;
				}
			}
			else {
				shotcool1 -= 5;
			}
		}
		if (myarg > 0) {
			myarg--;
		}
		else if (myarg < 0) {
			myarg++;
		}


		getData().score += static_cast<int32>(Cursor::Delta().length() * 10);
	}
	void draw() const override
	{
		Scene::SetBackground(ColorF(0.2, 0.8, 0.6));
		const double t = Scene::Time();
		//自キャラ1
		if (getData().num == 0) {
			//自機
			for (auto i : step(5))
			{
				const double theta = i * 72_deg + t * 180_deg;
				const Vec2 pos = OffsetCircular(player1, r, theta);
				Circle(pos.x, pos.y, 5).draw(HSV(i * 72 + t * 180));
				for (const auto& playerBullet : playerBullets)
				{
					Rect(playerBullet.x + pos.x - player1.x, playerBullet.y + pos.y - player1.y, 10, 20).draw(HSV(i * 72));
				}
			}
			Circle(player1, 10).draw(Palette::Orange);
			Rect(player1.x - 10, player1.y, 20, 1).draw(Palette::Yellow);
		}
		//自キャラ2
		if (getData().num == 1) {
			Circle(player1, 10).draw(Palette::Orange);
			Rect(player1.x - 10, player1.y, 20, 1).rotated(myarg * 1_deg).draw(Palette::Yellow);
			for (auto i : step(2))
			{
				const double theta = myarg * 1_deg + 90_deg + i * 180_deg + i * rush * 2_deg;

				const Vec2 pos = OffsetCircular(player1, r, theta);
				if (i == 0) {
					Circle(pos.x, pos.y- shotcool1, 7).draw(Palette::Rosybrown);
				}
				else {
					Circle(pos.x, pos.y, 7).draw(Palette::Rosybrown);
				}
			}
			for (const auto& playerBullet : playerBullets)
			{
				Circle(playerBullet, 8).draw(Palette::Orange);
			}
		}
		
		FontAsset(U"ScoreFont")(U"Score: {}"_fmt(getData().score)).draw(40, 40);
	}
};
void Main()
{
	FontAsset::Register(U"TitleFont", 60, Typeface::Heavy);
	FontAsset::Register(U"ScoreFont", 30, Typeface::Bold);
	// シーンマネージャーを作成
	App manager;

	// タイトルシーン（名前は U"Title"）を登録
	manager.add<Title>(U"Title");
	manager.add<Game>(U"Game");
	while (System::Update())
	{
		// 現在のシーンを実行
		if (!manager.update())
		{
			break;
		}
	}
}

//
// = アドバイス =
// Debug ビルドではプログラムの最適化がオフになります。
// 実行速度が遅いと感じた場合は Release ビルドを試しましょう。
// アプリをリリースするときにも、Release ビルドにするのを忘れないように！
//
// 思ったように動作しない場合は「デバッグの開始」でプログラムを実行すると、
// 出力ウィンドウに詳細なログが表示されるので、エラーの原因を見つけやすくなります。
//
// = お役立ちリンク | Quick Links =
//
// Siv3D リファレンス
// https://zenn.dev/reputeless/books/siv3d-documentation
//
// Siv3D Reference
// https://zenn.dev/reputeless/books/siv3d-documentation-en
//
// Siv3D コミュニティへの参加
// Slack や Twitter, BBS で気軽に質問や情報交換ができます。
// https://zenn.dev/reputeless/books/siv3d-documentation/viewer/community
//
// Siv3D User Community
// https://zenn.dev/reputeless/books/siv3d-documentation-en/viewer/community
//
// 新機能の提案やバグの報告 | Feedback
// https://github.com/Siv3D/OpenSiv3D/issues
//

/*
# include <Siv3D.hpp>

// 敵の位置をランダムに作成する関数
Vec2 GenerateEnemy()
{
    return RandomVec2({ 50, 750 }, -20);
}

void Main()
{
    Scene::SetBackground(ColorF(0.1, 0.2, 0.7));

    const Font font(30);

    // 自機テクスチャ
    const Texture playerTexture(Emoji(U"🤖"));
    // 敵テクスチャ
    const Texture enemyTexture(Emoji(U"👾"));

    // 自機
    Vec2 playerPos(400, 500);
    // 敵
    Array<Vec2> enemies = { GenerateEnemy() };

    // 自機ショット
    Array<Vec2> playerBullets;
    // 敵ショット
    Array<Vec2> enemyBullets;

    // 自機のスピード
    constexpr double playerSpeed = 550.0;
    // 自機ショットのスピード
    constexpr double playerBulletSpeed = 500.0;
    // 敵のスピード
    constexpr double enemySpeed = 100.0;
    // 敵ショットのスピード
    constexpr double enemyBulletSpeed = 300.0;

    // 敵の発生間隔の初期値（秒）
    double initialEnemySpawnTime = 2.0;
    // 敵の発生間隔（秒）
    double enemySpawnTime = initialEnemySpawnTime;
    // 敵の発生間隔タイマー
    double enemySpawnTimer = 0.0;

    // 自機ショットのクールタイム（秒）
    constexpr double playershotcool1Time = 0.1;
    // 自機ショットのクールタイムタイマー
    double playerShotTimer = 0.0;

    // 敵ショットのクールタイム（秒）
    constexpr double enemyshotcool1Time = 0.90;
    // 敵ショットのクールタイムタイマー
    double enemyShotTimer = 0.0;

    Effect effect;

    // ハイスコア
    int32 highScore = 0;
    // 現在のスコア
    int32 score = 0;

    while (System::Update())
    {
        // ゲームオーバー判定
        bool gameover = false;

        const double deltaTime = Scene::DeltaTime();
        enemySpawnTimer += deltaTime;
        playerShotTimer = Min(playerShotTimer + deltaTime, playershotcool1Time);
        enemyShotTimer += deltaTime;

        // 敵の発生
        while (enemySpawnTimer > enemySpawnTime)
        {
            enemySpawnTimer -= enemySpawnTime;
            enemySpawnTime = Max(enemySpawnTime * 0.95, 0.3);
            enemies << GenerateEnemy();
        }

        //-------------------
        //
        // 移動
        //

        // 自機の移動
        const Vec2 move = Vec2(KeyRight.pressed() - KeyLeft.pressed(), KeyDown.pressed() - KeyUp.pressed())
            .setLength(deltaTime * playerSpeed * (KeyShift.pressed() ? 0.5 : 1.0));
        playerPos.moveBy(move).clamp(Scene::Rect());

        // 自機ショットの発射
        if (playerShotTimer >= playershotcool1Time)
        {
            playerShotTimer = 0.0;
            playerBullets << playerPos.movedBy(0, -50);
        }

        // 自機ショットの移動
        for (auto& playerBullet : playerBullets)
        {
            playerBullet.y += deltaTime * -playerBulletSpeed;
        }
        // 画面外に出た自機ショットは消滅
        playerBullets.remove_if([](const Vec2& b) { return b.y < -40; });

        // 敵の移動
        for (auto& enemy : enemies)
        {
            enemy.y += deltaTime * enemySpeed;
        }
        // 画面外に出た敵は消滅
        enemies.remove_if([&](const Vec2& e)
        {
            if (e.y > 700)
            {
                // 敵が画面外に出たらゲームオーバー
                gameover = true;
                return true;
            }
            else
            {
                return false;
            }
        });

        // 敵ショットの発射
        if (enemyShotTimer >= enemyshotcool1Time)
        {
            enemyShotTimer -= enemyshotcool1Time;

            for (const auto& enemy : enemies)
            {
                enemyBullets << enemy;
            }
        }

        // 敵ショットの移動
        for (auto& enemyBullet : enemyBullets)
        {
            enemyBullet.y += deltaTime * enemyBulletSpeed;
        }
        // 画面外に出た自機ショットは消滅
        enemyBullets.remove_if([](const Vec2& b) {return b.y > 700; });

        //-------------------
        //
        // 攻撃判定
        //

        // 敵 vs 自機ショット
        for (auto itEnemy = enemies.begin(); itEnemy != enemies.end();)
        {
            const Circle enemyCircle(*itEnemy, 40);
            bool skip = false;

            for (auto itBullet = playerBullets.begin(); itBullet != playerBullets.end();)
            {
                if (enemyCircle.intersects(*itBullet))
                {
                    // 爆発エフェクトを追加
                    effect.add([pos = *itEnemy](double t)
                    {
                        const double t2 = (1.0 - t);
                        Circle(pos, 10 + t * 70).drawFrame(20 * t2, AlphaF(t2 * 0.5));
                        return t < 1.0;
                    });

                    itEnemy = enemies.erase(itEnemy);
                    playerBullets.erase(itBullet);
                    ++score;
                    skip = true;
                    break;
                }

                ++itBullet;
            }

            if (skip)
            {
                continue;
            }

            ++itEnemy;
        }

        // 敵ショット vs 自機
        for (const auto& enemyBullet : enemyBullets)
        {
            // 敵ショットが playerPos の 20 ピクセル以内に接近したら
            if (enemyBullet.distanceFrom(playerPos) <= 20)
            {
                // ゲームオーバーにする
                gameover = true;
                break;
            }
        }

        // ゲームオーバーならリセット
        if (gameover)
        {
            playerPos = Vec2(400, 500);
            enemies.clear();
            playerBullets.clear();
            enemyBullets.clear();
            enemySpawnTime = initialEnemySpawnTime;
            highScore = Max(highScore, score);
            score = 0;
        }

        //-------------------
        //
        // 描画
        //

        // 背景のアニメーション
        for (auto i : step(12))
        {
            const double a = Periodic::Sine0_1(2s, Scene::Time() - (2.0 / 12 * i));
            Rect(0, i * 50, 800, 50).draw(ColorF(1.0, a * 0.2));
        }

        // 自機の描画
        playerTexture.resized(80).flipped().drawAt(playerPos);

        // 自機ショットの描画
        for (const auto& playerBullet : playerBullets)
        {
            Circle(playerBullet, 8).draw(Palette::Orange);
        }

        // 敵の描画
        for (const auto& enemy : enemies)
        {
            enemyTexture.resized(60).drawAt(enemy);
        }

        // 敵ショットの描画
        for (const auto& enemyBullet : enemyBullets)
        {
            Circle(enemyBullet, 4).draw(Palette::White);
        }

        effect.update();

        // スコアの描画
        font(U"{} [{}]"_fmt(score, highScore)).draw(Arg::bottomRight(780, 580));
    }
}
*/
