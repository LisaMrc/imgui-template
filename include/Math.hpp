#pragma once

#include <imgui.h>
#include <ctime>
#include <random>

class Tools {
public:
    std::random_device rd;
    std::mt19937       rng = std::mt19937(rd());
};

class Binomial {
public:
    std::default_random_engine      engine = std::default_random_engine(static_cast<unsigned int>(time(0)));
    std::binomial_distribution<int> dist   = std::binomial_distribution<int>(2, 0.33); // 2 trials, 33% chance each
    bool                            done   = false;
    float                           result = .0f;

    void printQuote(ImFont* defaultFont, ImFont* customFont)
    {
        ImGui::PopFont();

        ImGui::PushFont(defaultFont);

        static const std::vector<std::string> quotes = {
            "“Le bonheur existe, il est dans l'amour”, Michelle Morgan",
            "“Je ne sais où va mon chemin mais je marche mieux quand ma main serre la tienne”, Alfred de Musset",
            "“Il n’y a rien de plus précieux en ce monde que le sentiment d’exister pour quelqu’un”, Victor Hugo",
            "“La plus grande vérité qu'on puisse apprendre un jour est qu'il suffit d'aimer et de l'être en retour.” Evan McGregor, Moulin Rouge",
            "“Aimer, c’est savoir dire je t’aime sans parler.”, Victor Hugo",
            "“L’amour est comme le vent, nous ne savons pas d’où il vient.”, Honoré de Balzac",
            "“Donnez à ceux que vous aimez des ailes pour voler, des racines pour revenir, et des raisons de rester.”, Dalaï Lama",
            "“S'aimer soi-même, c'est l'assurance d'une longue histoire d'amour.”, Oscar Wilde",
            "“Je ne dirai pas les raisons que tu as de m'aimer. Car tu n'en as point. La raison d'aimer, c'est l'amour”, Antoine de Saint-Exupéry",
            "“L'amour n'a point d'âge; il est toujours naissant. Les poètes l'ont dit.”, Blaise Pascal",
            "“Le pouvoir ne réside au fond que là où les hommes croient qu’il réside, le pouvoir n’est qu’un tour de passe-passe.”, Lord Varys, Game of Thrones",
            "“A qui peut se vaincre soi-même, il est peu de chose qui puisse résister.”, Louis XIV",
            "“La soif de dominer est celle qui s'éteint la dernière dans le coeur de l'homme.”, Nicolas Machiavel",
            "“Un acte de justice et de douceur a souvent plus de pouvoir sur le coeur des hommes que la violence et la barbarie”, Nicolas Machiavel",
            "“On gouverne mieux les hommes par leurs vices que par leurs vertus.”, Napoléon Bonaparte",
            "“Le mot de “vertu politique” est un non-sens.”, Napoléon Bonaparte",
            "“Un conciliateur c'est quelqu'un qui nourrit un crocodile en espérant qu'il sera le dernier à être mangé.”, Winston Churchill",
            "“Le pouvoir de l’homme s’est accru dans tous les domaines, excepté sur lui-même.”, Winston Churchill",
            "“Je n'ai jamais prôné la guerre, sauf comme moyen pour obtenir la paix.”, Ulysse S Grant",
            "“L’expérience, voilà le maître en toutes choses.”, Jules César",
            "“Chuck Norris ne ment pas, c'est la vérité qui se trompe.”, Chuck Norris",
            "“Mozart était tellement en avance sur son temps qu'à 35 ans il était déjà mort.”, Pierre Desproges",
            "“Chuck Norris est mort depuis 10 ans, mais la Mort n'a pas encore trouvé le courage d'aller lui dire.”, Chuck Norris",
            "“Certains disent : La violence ne résout rien, Chuck Norris leur répond : C'est que t'as pas tapé assez fort.”, Chuck Norris",
            "“Il faut cueillir les cerises avec la queue. J'avais déjà du mal avec la main !”, Coluche",
            "“Je voudrais rassurer les peuples qui meurent de faim dans le monde : ici, on mange pour vous.”, Coluche",
            "“Chuck Norris peut t'étrangler avec un téléphone sans fil.'” Chuck Norris",
            "“J’avais une maison au bord de la mer mais pour aller à la plage, il fallait passer devant un bar. Je n’ai jamais vu la mer.”, George Best",
            "“J’arrête de boire mais seulement quand je dors.”, George Best",
            "“Dieu a dit : Que la lumière soit ! Et Chuck Norris répondit : On dit s'il vous plait !”, Chuck Norris",
            "Believe in your move.",
            "Every square matters.",
            "Even the king started as a pawn.",
            "Patience is power.",
            "Think ahead. Always.",
            "« Ce n'est pas la taille de l'épreuve, mais la façon dont on la traverse qui compte. », A. A. Milne",
            "« La vie n'est pas d'attendre que l'orage passe, mais d'apprendre à danser sous la pluie. », Sénèque",
            "« Le but n'est pas de vivre éternellement, mais de créer quelque chose qui le fasse. », Chuck Palahniuk",
            "« Ce n'est pas ce que nous faisons, mais ce que nous choisissons de faire, qui détermine qui nous sommes. », J.K. Rowling",
            "« Les grandes choses ne sont jamais faites par impulsion, mais par une série de petites choses réunies. », Vincent van Gogh",
            "« Il est plus facile de détruire que de créer. Mais c'est dans la création que réside le vrai pouvoir. », Léo Tolstoï",
            "« L'art de vivre consiste à savoir souffrir et aimer, sans se laisser dominer par l'un ni l'autre. », Paul Valéry",
            "« La beauté sauvera le monde. », Fiodor Dostoïevski",
            "« Ne vous contentez pas de rêver votre vie, vivez vos rêves. », Albert Einstein",
            "« Le voyage de mille lieues commence par un pas. », Lao Tseu",
            "« Le silence est la réponse la plus profonde. », Maître Chan",
            "« Ne cherche pas à fuir tes pensées, mais à les comprendre. », Maître Dogen",
            "« Le chemin de la sagesse est une pratique sans fin. », Maître Xu Yun",
            "« Quand tu es dans l'instant présent, tu es en paix. », Maître Thich Nhat Hanh",
            "« Il n'y a pas de voie vers le bonheur. Le bonheur est la voie. », Maître Lao Tseu",
            "« L'esprit est tout. Ce que tu penses, tu deviens. », Bouddha",
            "« Ne te laisse pas influencer par la pensée du passé, ni par l'illusion du futur. Sois dans le moment présent. », Maître Hsing Yun",
            "« Rien n'est permanent, sauf le changement. », Maître Shunryu Suzuki",
            "« Lorsque tu fais une chose, fais-la de tout cœur. », Maître Chan",
            "« La véritable liberté vient de la maîtrise de soi. », Maître Xu Yun",
            "« Il n'y a pas de chemin vers la paix, la paix est le chemin. », Bouddha",
            "« L'illumination ne se trouve pas dans l'extérieur, mais dans l'intérieur. », Maître Dogen",
            "« La vraie sagesse réside dans la simplicité. », Maître Shaolin",
            "« Ce que tu cherches à l'extérieur, tu le possèdes déjà à l'intérieur. », Maître Hsing Yun",
            "« La souffrance fait partie de la vie, mais elle n'est pas la vie. », Maître Thich Nhat Hanh",
            "« Quand l'esprit est pur, la voie est claire. », Maître Chan",
            "« Le maître ne montre pas la voie, il montre la manière de marcher. », Maître Nan Huai Chin",
            "« Il n'y a pas de maître en dehors de soi. », Maître Bodhidharma",
            "« Le plus grand enseignement est celui qui est directement expérimenté, sans mots. », Maître Shunryu Suzuki",
            "« L'ignorance est la source de la souffrance. La sagesse est la clé de la liberté. », Maître Thich Nhat Hanh",
            "« L'esprit éveillé est celui qui est calme et détaché, sans désir. », Maître Xu Yun",
            "« La vérité ne peut être atteinte que par la pratique directe, au-delà des mots. », Maître Bodhidharma",
            "« La paix vient de l'acceptation de l'impermanence de tout. », Maître Hsing Yun",
            "« Ne cherche pas la perfection dans l'extérieur, mais dans ton propre esprit. », Maître Chan",
            "« Quand tu médites, sois comme une montagne, stable et sans mouvement. », Maître Dogen",
            "« Ne pense pas à l'ego, pense à la voie. », Maître Shunryu Suzuki",
            "« La pratique de la pleine conscience est l'éveil à la réalité. », Maître Thich Nhat Hanh",
            "« La sagesse n'est pas une accumulation, mais une dissolution de l'illusion. », Maître Chan",
            "« Le véritable combat est contre notre propre ignorance. », Maître Xu Yun",
            "« L'illusion naît de l'attachement, la libération vient du lâcher-prise. », Maître Bodhidharma",
            "« Ne te laisse pas emporter par tes pensées, elles ne sont que des nuages dans le ciel. », Maître Hsing Yun",
            "« La voie du guerrier est aussi la voie du moine : une recherche de la vérité intérieure. », Maître Shaolin",
            "« Quand tu pratiques, sois détaché des résultats, concentre-toi uniquement sur l'instant. », Maître Dogen",
            "« Le calme intérieur est la seule voie vers la paix extérieure. », Maître Thich Nhat Hanh",
            "« Les vagues de l'esprit ne se dissipent que lorsque l'on cesse de les nourrir. », Maître Chan",
            "« Accepter ce qui est, tel qu'il est, voilà l'essence de l'éveil. », Maître Shunryu Suzuki",
            "« Ne cherche pas à comprendre le monde extérieur, mais à comprendre ton propre cœur. », Maître Hsing Yun",
            "« L'illusion de soi est la source de toute souffrance. L'éveil commence par la reconnaissance de cette illusion. », Maître Bodhidharma",
            "« La voie du Shaolin est une voie de discipline, de calme et de recherche intérieure. », Maître Shaolin",
            "« La véritable pratique ne réside pas dans la méditation, mais dans la mise en pratique de ce que l'on comprend. », Maître Xu Yun",
            "« L'éveil n'est pas un lieu où aller, mais un état d'esprit à cultiver. », Maître Xu Yun",
            "« Ne cherche pas la voie en dehors de toi-même. Elle réside déjà dans ton cœur. », Maître Dogen",
            "« La sagesse est silencieuse. Elle ne se trouve pas dans les mots, mais dans l'expérience. », Maître Shunryu Suzuki",
            "« Tout ce qui existe dans le monde est impermanent, y compris notre propre esprit. », Maître Hsing Yun",
            "« La pratique est le miroir de notre esprit. Ce que nous voyons est le reflet de ce que nous sommes. », Maître Bodhidharma",
            "« Les souffrances de l'âme ne peuvent être guéries que par la méditation et le lâcher-prise. », Maître Thich Nhat Hanh",
            "« Accepte ce qui vient et laisse partir ce qui part, tout en restant centré. », Maître Chan",
            "« Le véritable combattant n'est pas celui qui détruit, mais celui qui se conquiert lui-même. », Maître Shaolin",
            "« Ne cherche pas l'illumination comme un objectif. Sois l'illumination dans chaque instant. », Maître Dogen",
            "« L'esprit en paix est un esprit sans jugement. », Maître Hsing Yun",
            "« La pratique du Chan ne consiste pas à ajouter, mais à éliminer ce qui n'est pas nécessaire. », Maître Bodhidharma",
            "« Il n'y a pas de chemin vers l'éveil, il n'y a que l'éveil du chemin. », Maître Thich Nhat Hanh",
            "« La simplicité de la voie est ce qui la rend si profonde. », Maître Shunryu Suzuki",
            "« L'esprit est comme un miroir. Si tu le nettoies, il te montre la vérité. », Maître Chan",
            "« La sagesse naît du silence, pas des mots. », Maître Xu Yun",
            "« La véritable liberté est celle qui se trouve en toi, pas dans le monde extérieur. », Maître Bodhidharma",
            "« Pratique sans interruption, mais sans aucune attente de résultat. », Maître Hsing Yun",
            "« Si tu veux la paix, commence par la cultiver dans ton cœur. », Maître Dogen",
            "« La force de l'esprit ne réside pas dans sa rigidité, mais dans sa flexibilité. », Maître Shaolin",
            "« La voie du Shaolin est une voie de calme, de maîtrise de soi et de clarté intérieure. », Maître Chan"
        };

        static int idx = rand() % quotes.size();
        ImGui::Text("%s", quotes[idx].c_str());

        ImGui::PopFont();

        ImGui::PushFont(customFont);
    }
};

class Exponential {
public:
    std::exponential_distribution<> dist = std::exponential_distribution<>(0.14); // 90% chance of a piece in the [0, 16] range being removed
    bool                            done = false;
};

class Gamma {
public:
    std::gamma_distribution<double> dist = std::gamma_distribution<double>(4, 7.5);
    bool                            done = false;
};

class Bernoulli {
public:
    std::default_random_engine  engine; // Random engine
    std::bernoulli_distribution dist;   // Bernoulli distribution

    bool done   = false;
    bool result = false;

    Bernoulli()
        : engine(static_cast<unsigned int>(time(0))) {}

    // Method to perform the Bernoulli trial and return the result
    bool flip(double probability)
    {
        dist   = std::bernoulli_distribution(probability);
        result = dist(engine);
        return result;
    }

    bool getResult() const
    {
        return result;
    }
};
