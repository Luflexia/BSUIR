--
-- PostgreSQL database dump
--

-- Dumped from database version 17.2
-- Dumped by pg_dump version 17.2

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: Билет; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Билет" (
    id integer NOT NULL,
    "Пассажир_id" integer NOT NULL,
    "Поезд_id" integer NOT NULL,
    "Маршрут_id" integer NOT NULL,
    "Место" integer NOT NULL,
    "Время_покупки" timestamp without time zone DEFAULT now() NOT NULL,
    "Стоимость" numeric(10,2),
    "Категория" character varying(50)
);


ALTER TABLE public."Билет" OWNER TO postgres;

--
-- Name: Билет_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public."Билет_id_seq"
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public."Билет_id_seq" OWNER TO postgres;

--
-- Name: Билет_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public."Билет_id_seq" OWNED BY public."Билет".id;


--
-- Name: Маршрут; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Маршрут" (
    id integer NOT NULL,
    "Время_в_пути" interval NOT NULL,
    "Количество_остановок" integer,
    "Название" character varying NOT NULL,
    "Расстояние" numeric NOT NULL
);


ALTER TABLE public."Маршрут" OWNER TO postgres;

--
-- Name: Маршрут_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public."Маршрут_id_seq"
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public."Маршрут_id_seq" OWNER TO postgres;

--
-- Name: Маршрут_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public."Маршрут_id_seq" OWNED BY public."Маршрут".id;


--
-- Name: Маршрут_Станция; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Маршрут_Станция" (
    id integer NOT NULL,
    "Маршрут_id" integer NOT NULL,
    "Станция_id" integer NOT NULL,
    "Порядок_остановки" integer NOT NULL
);


ALTER TABLE public."Маршрут_Станция" OWNER TO postgres;

--
-- Name: Маршрут_Станция_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public."Маршрут_Станция_id_seq"
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public."Маршрут_Станция_id_seq" OWNER TO postgres;

--
-- Name: Маршрут_Станция_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public."Маршрут_Станция_id_seq" OWNED BY public."Маршрут_Станция".id;


--
-- Name: Пассажир; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Пассажир" (
    id integer NOT NULL,
    "ФИО" character varying(255) NOT NULL,
    "Номер_паспорта" character varying(50) NOT NULL,
    "Пол" character varying(10),
    "Контактный_телефон" character varying(20) NOT NULL
);


ALTER TABLE public."Пассажир" OWNER TO postgres;

--
-- Name: Пассажир_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public."Пассажир_id_seq"
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public."Пассажир_id_seq" OWNER TO postgres;

--
-- Name: Пассажир_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public."Пассажир_id_seq" OWNED BY public."Пассажир".id;


--
-- Name: Поезд; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Поезд" (
    id integer NOT NULL,
    "Количество_вагонов" integer,
    "Вместимость" integer,
    "Компания" character varying(100) NOT NULL,
    "Тип" character varying(50)
);


ALTER TABLE public."Поезд" OWNER TO postgres;

--
-- Name: Поезд_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public."Поезд_id_seq"
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public."Поезд_id_seq" OWNER TO postgres;

--
-- Name: Поезд_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public."Поезд_id_seq" OWNED BY public."Поезд".id;


--
-- Name: Поезд_Сотрудник; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Поезд_Сотрудник" (
    id integer NOT NULL,
    "Поезд_id" integer NOT NULL,
    "Сотрудник_id" integer NOT NULL
);


ALTER TABLE public."Поезд_Сотрудник" OWNER TO postgres;

--
-- Name: Поезд_Сотрудник_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public."Поезд_Сотрудник_id_seq"
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public."Поезд_Сотрудник_id_seq" OWNER TO postgres;

--
-- Name: Поезд_Сотрудник_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public."Поезд_Сотрудник_id_seq" OWNED BY public."Поезд_Сотрудник".id;


--
-- Name: Расписание; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Расписание" (
    id integer NOT NULL,
    "Поезд_id" integer NOT NULL,
    "Маршрут_id" integer NOT NULL,
    "Станция_отбытия_id" integer NOT NULL,
    "Станция_прибытия_id" integer NOT NULL,
    "Время_отбытия" time without time zone NOT NULL,
    "Время_прибытия" time without time zone NOT NULL
);


ALTER TABLE public."Расписание" OWNER TO postgres;

--
-- Name: Расписание_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public."Расписание_id_seq"
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public."Расписание_id_seq" OWNER TO postgres;

--
-- Name: Расписание_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public."Расписание_id_seq" OWNED BY public."Расписание".id;


--
-- Name: Сотрудник; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Сотрудник" (
    id integer NOT NULL,
    "ФИО" character varying(255) NOT NULL,
    "Контактный_телефон" character varying(20) NOT NULL,
    "Должность" character varying(100) NOT NULL,
    "Стаж" integer
);


ALTER TABLE public."Сотрудник" OWNER TO postgres;

--
-- Name: Сотрудник_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public."Сотрудник_id_seq"
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public."Сотрудник_id_seq" OWNER TO postgres;

--
-- Name: Сотрудник_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public."Сотрудник_id_seq" OWNED BY public."Сотрудник".id;


--
-- Name: Сотрудник_Маршрут; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Сотрудник_Маршрут" (
    id integer NOT NULL,
    "Сотрудник_id" integer NOT NULL,
    "Маршрут_id" integer NOT NULL
);


ALTER TABLE public."Сотрудник_Маршрут" OWNER TO postgres;

--
-- Name: Сотрудник_Маршрут_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public."Сотрудник_Маршрут_id_seq"
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public."Сотрудник_Маршрут_id_seq" OWNER TO postgres;

--
-- Name: Сотрудник_Маршрут_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public."Сотрудник_Маршрут_id_seq" OWNED BY public."Сотрудник_Маршрут".id;


--
-- Name: Станция; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Станция" (
    id integer NOT NULL,
    "Название" character varying(100) NOT NULL,
    "Город" character varying(100) NOT NULL,
    "Количество_платформ" integer,
    "Количество_путей" integer
);


ALTER TABLE public."Станция" OWNER TO postgres;

--
-- Name: Станция_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public."Станция_id_seq"
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public."Станция_id_seq" OWNER TO postgres;

--
-- Name: Станция_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public."Станция_id_seq" OWNED BY public."Станция".id;


--
-- Name: Билет id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Билет" ALTER COLUMN id SET DEFAULT nextval('public."Билет_id_seq"'::regclass);


--
-- Name: Маршрут id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Маршрут" ALTER COLUMN id SET DEFAULT nextval('public."Маршрут_id_seq"'::regclass);


--
-- Name: Маршрут_Станция id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Маршрут_Станция" ALTER COLUMN id SET DEFAULT nextval('public."Маршрут_Станция_id_seq"'::regclass);


--
-- Name: Пассажир id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Пассажир" ALTER COLUMN id SET DEFAULT nextval('public."Пассажир_id_seq"'::regclass);


--
-- Name: Поезд id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Поезд" ALTER COLUMN id SET DEFAULT nextval('public."Поезд_id_seq"'::regclass);


--
-- Name: Поезд_Сотрудник id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Поезд_Сотрудник" ALTER COLUMN id SET DEFAULT nextval('public."Поезд_Сотрудник_id_seq"'::regclass);


--
-- Name: Расписание id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Расписание" ALTER COLUMN id SET DEFAULT nextval('public."Расписание_id_seq"'::regclass);


--
-- Name: Сотрудник id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Сотрудник" ALTER COLUMN id SET DEFAULT nextval('public."Сотрудник_id_seq"'::regclass);


--
-- Name: Сотрудник_Маршрут id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Сотрудник_Маршрут" ALTER COLUMN id SET DEFAULT nextval('public."Сотрудник_Маршрут_id_seq"'::regclass);


--
-- Name: Станция id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Станция" ALTER COLUMN id SET DEFAULT nextval('public."Станция_id_seq"'::regclass);


--
-- Data for Name: Билет; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Билет" (id, "Пассажир_id", "Поезд_id", "Маршрут_id", "Место", "Время_покупки", "Стоимость", "Категория") FROM stdin;
1	1	3	2	12	2025-02-20 10:30:00	80.50	Эконом
2	2	5	4	8	2025-02-18 14:15:00	155.00	Бизнес
3	3	7	1	22	2025-02-19 09:00:00	100.00	Эконом
4	4	2	5	5	2025-02-17 11:45:00	90.75	Плацкарт
5	5	1	3	18	2025-02-16 13:20:00	180.00	Купе
6	6	4	7	9	2025-02-21 16:50:00	135.50	Бизнес
7	7	6	6	15	2025-02-22 12:00:00	75.00	Эконом
8	8	9	2	7	2025-02-23 18:25:00	145.00	Купе
9	9	11	4	10	2025-02-20 07:40:00	95.25	Плацкарт
10	10	8	1	3	2025-02-19 19:30:00	160.00	Бизнес
11	11	13	5	6	2025-02-18 15:00:00	85.00	Эконом
12	12	15	3	20	2025-02-21 08:10:00	100.50	Купе
13	13	10	2	11	2025-02-17 17:25:00	82.30	Эконом
14	14	12	7	14	2025-02-22 09:45:00	130.00	Бизнес
15	15	16	6	2	2025-02-20 11:20:00	97.40	Плацкарт
16	16	14	1	19	2025-02-23 14:00:00	150.00	Купе
17	17	17	4	4	2025-02-19 10:35:00	75.60	Эконом
18	18	19	5	13	2025-02-21 13:55:00	125.00	Бизнес
19	19	20	2	21	2025-02-18 16:40:00	90.00	Эконом
20	20	22	3	16	2025-02-17 12:30:00	140.00	Купе
21	21	24	7	1	2025-02-22 10:10:00	78.90	Эконом
22	22	25	6	23	2025-02-20 15:45:00	160.50	Бизнес
23	23	27	1	6	2025-02-18 09:20:00	99.90	Плацкарт
24	24	29	5	17	2025-02-19 17:00:00	110.00	Купе
25	25	30	2	14	2025-02-21 11:35:00	87.75	Эконом
26	26	28	4	5	2025-02-23 08:50:00	125.30	Бизнес
27	27	26	7	9	2025-02-20 18:20:00	92.40	Плацкарт
28	28	23	6	20	2025-02-22 13:25:00	138.00	Купе
29	29	21	3	2	2025-02-19 07:10:00	80.00	Эконом
30	30	18	1	11	2025-02-18 12:50:00	155.60	Бизнес
31	31	10	5	8	2025-02-23 16:40:00	99.50	Купе
\.


--
-- Data for Name: Маршрут; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Маршрут" (id, "Время_в_пути", "Количество_остановок", "Название", "Расстояние") FROM stdin;
1	01:20:00	2	Минск - Молодечно	75
2	02:45:00	4	Минск - Витебск	280
3	03:30:00	5	Минск - Гродно	310
4	04:00:00	6	Минск - Брест	350
5	02:15:00	3	Минск - Гомель	295
6	01:50:00	2	Минск - Могилёв	200
7	03:00:00	4	Гомель - Брянск	230
8	06:20:00	8	Минск - Москва	720
9	06:00:00	7	Минск - Санкт-Петербург	790
10	01:30:00	2	Гродно - Лида	115
11	02:00:00	3	Брест - Пинск	180
12	04:50:00	6	Минск - Калининград	530
13	05:10:00	7	Минск - Смоленск	340
14	02:40:00	3	Могилёв - Орша	140
15	03:20:00	4	Гомель - Киев	270
16	05:40:00	7	Витебск - Псков	400
17	04:30:00	5	Минск - Вильнюс	190
18	07:00:00	9	Минск - Рига	470
19	03:50:00	5	Минск - Каунас	270
20	04:10:00	6	Минск - Варшава	550
21	05:20:00	7	Минск - Львов	600
22	03:10:00	4	Гродно - Белосток	180
23	06:00:00	8	Минск - Одесса	900
24	02:50:00	4	Витебск - Смоленск	135
25	04:00:00	5	Брест - Люблин	250
26	04:20:00	6	Минск - Житомир	430
27	02:30:00	3	Минск - Бобруйск	150
28	03:40:00	5	Могилёв - Гомель	240
29	05:50:00	7	Гродно - Калининград	380
30	01:10:00	2	Минск - Заславль	25
31	06:30:00	8	Минск - Чернигов	510
\.


--
-- Data for Name: Маршрут_Станция; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Маршрут_Станция" (id, "Маршрут_id", "Станция_id", "Порядок_остановки") FROM stdin;
1	1	1	1
2	1	5	2
3	1	10	3
4	1	15	4
5	1	20	5
6	2	2	1
7	2	6	2
8	2	11	3
9	2	16	4
10	2	21	5
11	3	3	1
12	3	7	2
13	3	12	3
14	3	17	4
15	3	22	5
16	4	4	1
17	4	8	2
18	4	13	3
19	4	18	4
20	4	23	5
21	5	5	1
22	5	9	2
23	5	14	3
24	5	19	4
25	5	24	5
26	6	1	1
27	6	11	2
28	6	21	3
29	6	26	4
30	6	31	5
31	7	2	1
\.


--
-- Data for Name: Пассажир; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Пассажир" (id, "ФИО", "Номер_паспорта", "Пол", "Контактный_телефон") FROM stdin;
1	Иванов Иван Иванович	AB1234567	Мужской	+375291234567
2	Петров Петр Петрович	AB2234567	Мужской	+375292234567
3	Сидорова Анна Викторовна	AB3234567	Женский	+375293234567
4	Козлов Василий Сергеевич	AB4234567	Мужской	+375294234567
5	Смирнова Ольга Андреевна	AB5234567	Женский	+375295234567
6	Васильев Артем Николаевич	AB6234567	Мужской	+375296234567
7	Коваленко Дарья Алексеевна	AB7234567	Женский	+375297234567
8	Дмитриев Алексей Олегович	AB8234567	Мужской	+375298234567
9	Федорова Марина Павловна	AB9234567	Женский	+375299234567
10	Романов Сергей Викторович	BB1234567	Мужской	+375291122233
11	Лебедева Екатерина Сергеевна	BB2234567	Женский	+375292223344
12	Орлов Николай Иванович	BB3234567	Мужской	+375293334455
13	Гусева Татьяна Владимировна	BB4234567	Женский	+375294445566
14	Егоров Максим Александрович	BB5234567	Мужской	+375295556677
15	Захарова Светлана Дмитриевна	BB6234567	Женский	+375296667788
16	Кириллов Павел Станиславович	BB7234567	Мужской	+375297778899
17	Мельникова Юлия Андреевна	BB8234567	Женский	+375298889900
18	Тимофеев Игорь Владимирович	BB9234567	Мужской	+375299990011
19	Сорокина Надежда Сергеевна	CC1234567	Женский	+375291112233
20	Борисов Аркадий Геннадьевич	CC2234567	Мужской	+375292223344
21	Никитина Лариса Ивановна	CC3234567	Женский	+375293334455
22	Савельев Дмитрий Викторович	CC4234567	Мужской	+375294445566
23	Фролова Оксана Владимировна	CC5234567	Женский	+375295556677
24	Рябов Евгений Анатольевич	CC6234567	Мужской	+375296667788
25	Полякова Мария Олеговна	CC7234567	Женский	+375297778899
26	Щербаков Алексей Павлович	CC8234567	Мужской	+375298889900
27	Шевченко Галина Андреевна	CC9234567	Женский	+375299990011
28	Федоров Николай Семенович	DD1234567	Мужской	+375291112244
29	Емельянова Ольга Константиновна	DD2239567	Женский	+375299997011
30	Сорокина Галина Андреевна	CC9664566	Женский	+375288990011
31	Барсук Николай Семенович	DD1114567	Мужской	+375295512244
\.


--
-- Data for Name: Поезд; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Поезд" (id, "Количество_вагонов", "Вместимость", "Компания", "Тип") FROM stdin;
1	20	800	РЖД	Скоростной
2	15	600	РЖД	Пассажирский
3	12	500	БелЖД	Пассажирский
4	10	400	РЖД	Пригородный
5	18	720	РЖД	Скоростной
6	14	560	БелЖД	Эспресс
7	16	640	РЖД	Ночной
8	8	320	РЖД	Пригородный
9	22	880	РЖД	Скоростной
10	19	760	РЖД	Пассажирский
11	11	440	БелЖД	Пригородный
12	17	680	РЖД	Ночной
13	13	520	РЖД	Пассажирский
14	9	360	БелЖД	Пригородный
15	21	840	РЖД	Скоростной
16	12	480	РЖД	Пассажирский
17	10	400	РЖД	Пригородный
18	16	640	РЖД	Ночной
19	18	720	БелЖД	Экспресс
20	14	560	РЖД	Пассажирский
21	20	800	РЖД	Скоростной
22	15	600	БелЖД	Пассажирский
23	11	440	РЖД	Пригородный
24	17	680	РЖД	Ночной
25	13	520	РЖД	Пассажирский
26	9	360	БелЖД	Пригородный
27	22	880	РЖД	Скоростной
28	19	760	РЖД	Пассажирский
29	8	320	БелЖД	Пригородный
30	18	720	РЖД	Экспресс
31	14	560	РЖД	Пассажирский
\.


--
-- Data for Name: Поезд_Сотрудник; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Поезд_Сотрудник" (id, "Поезд_id", "Сотрудник_id") FROM stdin;
1	1	1
2	1	2
3	1	3
4	2	4
5	2	5
6	2	6
7	3	7
8	3	8
9	3	9
10	4	10
11	4	11
12	4	12
13	5	13
14	5	14
15	5	15
16	6	16
17	6	17
18	6	18
19	7	19
20	7	20
21	7	21
22	8	22
23	8	23
24	8	24
25	9	25
26	9	26
27	9	27
28	10	28
29	10	29
30	10	30
31	11	31
\.


--
-- Data for Name: Расписание; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Расписание" (id, "Поезд_id", "Маршрут_id", "Станция_отбытия_id", "Станция_прибытия_id", "Время_отбытия", "Время_прибытия") FROM stdin;
1	1	1	1	2	08:00:00	09:20:00
2	2	2	1	3	07:30:00	10:15:00
3	3	3	1	4	06:00:00	09:30:00
4	4	4	1	5	10:00:00	14:00:00
5	5	5	1	6	09:00:00	11:15:00
6	6	6	1	7	12:00:00	13:50:00
7	7	7	6	8	14:30:00	17:30:00
8	8	8	1	9	23:00:00	05:20:00
9	9	9	1	10	21:00:00	03:00:00
10	10	10	4	11	08:00:00	09:30:00
11	11	11	5	12	07:00:00	09:00:00
12	12	12	1	13	13:00:00	17:50:00
13	13	13	1	14	06:30:00	11:40:00
14	14	14	7	15	10:15:00	12:55:00
15	15	15	6	16	07:45:00	11:05:00
16	16	16	1	17	09:30:00	14:00:00
17	17	17	1	18	22:00:00	05:00:00
18	18	18	1	19	15:00:00	18:50:00
19	19	19	1	20	16:00:00	20:10:00
20	20	20	1	21	05:00:00	09:10:00
21	21	21	1	22	07:00:00	12:20:00
22	22	22	4	23	09:20:00	12:30:00
23	23	23	1	24	21:30:00	04:00:00
24	24	24	3	25	08:15:00	10:45:00
25	25	25	5	26	12:00:00	16:00:00
26	26	26	1	27	06:50:00	11:10:00
27	27	27	2	28	14:00:00	16:30:00
28	28	28	7	29	07:30:00	11:10:00
29	29	29	4	30	08:00:00	09:10:00
30	30	30	1	31	06:00:00	12:30:00
31	31	31	1	2	18:00:00	00:30:00
\.


--
-- Data for Name: Сотрудник; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Сотрудник" (id, "ФИО", "Контактный_телефон", "Должность", "Стаж") FROM stdin;
1	Иванов Александр Петрович	+375291234567	Машинист	12
2	Смирнова Елена Викторовна	+375331112233	Проводник	8
3	Кузнецов Дмитрий Сергеевич	+375441223344	Диспетчер	15
4	Новикова Мария Ивановна	+375291112233	Кассир	5
5	Попов Алексей Андреевич	+375291234568	Проводник	10
6	Лебедева Ольга Николаевна	+375441112244	Машинист	20
7	Соколов Сергей Дмитриевич	+375291223344	Проводник	7
8	Ковалева Анна Петровна	+375333334455	Диспетчер	9
9	Морозов Николай Игоревич	+375445556677	Кассир	4
10	Павлова Екатерина Сергеевна	+375291334455	Проводник	11
11	Романов Иван Николаевич	+375441445566	Машинист	18
12	Григорьева Оксана Владимировна	+375292223344	Диспетчер	13
13	Васильев Андрей Павлович	+375445667788	Проводник	6
14	Зайцева Ирина Васильевна	+375291556677	Кассир	3
15	Петрова Светлана Алексеевна	+375291667788	Проводник	9
16	Борисов Артем Олегович	+375441778899	Машинист	21
17	Тарасова Дарья Ильинична	+375291778899	Проводник	10
18	Гусев Максим Сергеевич	+375331445566	Диспетчер	17
19	Мельникова Алина Евгеньевна	+375441889900	Кассир	2
20	Федоров Олег Владимирович	+375291889900	Проводник	14
21	Козлова Наталья Андреевна	+375441990011	Машинист	23
22	Алексеев Сергей Викторович	+375331223344	Диспетчер	16
23	Макарова Юлия Николаевна	+375445001122	Кассир	5
24	Егоров Роман Петрович	+375292334455	Проводник	12
25	Никитина Татьяна Сергеевна	+375291112211	Машинист	19
26	Орлов Артем Дмитриевич	+375331334455	Диспетчер	8
27	Семенова Ксения Павловна	+375291445566	Проводник	7
28	Михайлов Иван Алексеевич	+375445112233	Кассир	4
29	Белова Анастасия Викторовна	+375331556677	Проводник	11
30	Волков Алексей Сергеевич	+375291667799	Машинист	25
31	Савельева Марина Игоревна	+375441667788	Диспетчер	6
\.


--
-- Data for Name: Сотрудник_Маршрут; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Сотрудник_Маршрут" (id, "Сотрудник_id", "Маршрут_id") FROM stdin;
1	1	1
2	2	1
3	3	1
4	4	2
5	5	2
6	6	2
7	7	3
8	8	3
9	9	3
10	10	4
11	11	4
12	12	4
13	13	5
14	14	5
15	15	5
16	16	6
17	17	6
18	18	6
19	19	7
20	20	7
21	21	7
22	22	8
23	23	8
24	24	8
25	25	9
26	26	9
27	27	9
28	28	10
29	29	10
30	30	10
31	31	11
\.


--
-- Data for Name: Станция; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Станция" (id, "Название", "Город", "Количество_платформ", "Количество_путей") FROM stdin;
1	Ленинградский вокзал	Москва	9	16
2	Киевский вокзал	Москва	8	14
3	Белорусский вокзал	Москва	7	15
4	Курский вокзал	Москва	10	18
5	Витебский вокзал	Санкт-Петербург	7	13
6	Московский вокзал	Санкт-Петербург	9	17
7	Финляндский вокзал	Санкт-Петербург	6	12
8	Каланчёвская	Москва	4	8
9	Новосибирск-Главный	Новосибирск	10	16
10	Екатеринбург-Пассажирский	Екатеринбург	8	14
11	Минск-Пассажирский	Минск	9	15
12	Гомель-Пассажирский	Гомель	6	10
13	Брест-Центральный	Брест	7	12
14	Гродно-Пассажирский	Гродно	5	9
15	Витебск-Пассажирский	Витебск	6	11
16	Могилёв-1	Могилёв	5	10
17	Смоленск-Центральный	Смоленск	6	11
18	Псков-Пассажирский	Псков	4	9
19	Калининград-Южный	Калининград	5	10
20	Мурманск-Пассажирский	Мурманск	5	9
21	Архангельск-Город	Архангельск	4	8
22	Великие Луки	Великие Луки	3	7
23	Тверь-Пассажирская	Тверь	4	9
24	Ярославль-Главный	Ярославль	6	11
25	Казань-Пассажирская	Казань	8	14
26	Уфа-Пассажирская	Уфа	7	13
27	Петрозаводск-Пассажирский	Петрозаводск	4	9
28	Воронеж-1	Воронеж	6	12
29	Ростов-Главный	Ростов-на-Дону	9	15
30	Краснодар-Главный	Краснодар	8	14
31	Сочи	Анапа	7	13
\.


--
-- Name: Билет_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Билет_id_seq"', 31, true);


--
-- Name: Маршрут_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Маршрут_id_seq"', 31, true);


--
-- Name: Маршрут_Станция_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Маршрут_Станция_id_seq"', 31, true);


--
-- Name: Пассажир_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Пассажир_id_seq"', 31, true);


--
-- Name: Поезд_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Поезд_id_seq"', 31, true);


--
-- Name: Поезд_Сотрудник_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Поезд_Сотрудник_id_seq"', 31, true);


--
-- Name: Расписание_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Расписание_id_seq"', 31, true);


--
-- Name: Сотрудник_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Сотрудник_id_seq"', 31, true);


--
-- Name: Сотрудник_Маршрут_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Сотрудник_Маршрут_id_seq"', 31, true);


--
-- Name: Станция_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Станция_id_seq"', 31, true);


--
-- Name: Билет Билет_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Билет"
    ADD CONSTRAINT "Билет_pkey" PRIMARY KEY (id);


--
-- Name: Маршрут Маршрут_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Маршрут"
    ADD CONSTRAINT "Маршрут_pkey" PRIMARY KEY (id);


--
-- Name: Маршрут_Станция Маршрут_Станция_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Маршрут_Станция"
    ADD CONSTRAINT "Маршрут_Станция_pkey" PRIMARY KEY (id);


--
-- Name: Пассажир Пассажир_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Пассажир"
    ADD CONSTRAINT "Пассажир_pkey" PRIMARY KEY (id);


--
-- Name: Пассажир Пассажир_Номер_паспорта_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Пассажир"
    ADD CONSTRAINT "Пассажир_Номер_паспорта_key" UNIQUE ("Номер_паспорта");


--
-- Name: Поезд Поезд_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Поезд"
    ADD CONSTRAINT "Поезд_pkey" PRIMARY KEY (id);


--
-- Name: Поезд_Сотрудник Поезд_Сотрудник_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Поезд_Сотрудник"
    ADD CONSTRAINT "Поезд_Сотрудник_pkey" PRIMARY KEY (id);


--
-- Name: Расписание Расписание_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Расписание"
    ADD CONSTRAINT "Расписание_pkey" PRIMARY KEY (id);


--
-- Name: Сотрудник Сотрудник_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Сотрудник"
    ADD CONSTRAINT "Сотрудник_pkey" PRIMARY KEY (id);


--
-- Name: Сотрудник_Маршрут Сотрудник_Маршрут_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Сотрудник_Маршрут"
    ADD CONSTRAINT "Сотрудник_Маршрут_pkey" PRIMARY KEY (id);


--
-- Name: Станция Станция_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Станция"
    ADD CONSTRAINT "Станция_pkey" PRIMARY KEY (id);


--
-- Name: Билет Билет_Маршрут_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Билет"
    ADD CONSTRAINT "Билет_Маршрут_id_fkey" FOREIGN KEY ("Маршрут_id") REFERENCES public."Маршрут"(id) ON DELETE CASCADE;


--
-- Name: Билет Билет_Пассажир_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Билет"
    ADD CONSTRAINT "Билет_Пассажир_id_fkey" FOREIGN KEY ("Пассажир_id") REFERENCES public."Пассажир"(id) ON DELETE CASCADE;


--
-- Name: Билет Билет_Поезд_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Билет"
    ADD CONSTRAINT "Билет_Поезд_id_fkey" FOREIGN KEY ("Поезд_id") REFERENCES public."Поезд"(id) ON DELETE CASCADE;


--
-- Name: Маршрут_Станция Маршрут_Станция_Маршрут_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Маршрут_Станция"
    ADD CONSTRAINT "Маршрут_Станция_Маршрут_id_fkey" FOREIGN KEY ("Маршрут_id") REFERENCES public."Маршрут"(id) ON DELETE CASCADE;


--
-- Name: Маршрут_Станция Маршрут_Станция_Станция_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Маршрут_Станция"
    ADD CONSTRAINT "Маршрут_Станция_Станция_id_fkey" FOREIGN KEY ("Станция_id") REFERENCES public."Станция"(id) ON DELETE CASCADE;


--
-- Name: Поезд_Сотрудник Поезд_Сотрудник_Поезд_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Поезд_Сотрудник"
    ADD CONSTRAINT "Поезд_Сотрудник_Поезд_id_fkey" FOREIGN KEY ("Поезд_id") REFERENCES public."Поезд"(id) ON DELETE CASCADE;


--
-- Name: Поезд_Сотрудник Поезд_Сотрудник_Сотрудник_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Поезд_Сотрудник"
    ADD CONSTRAINT "Поезд_Сотрудник_Сотрудник_id_fkey" FOREIGN KEY ("Сотрудник_id") REFERENCES public."Сотрудник"(id) ON DELETE CASCADE;


--
-- Name: Расписание Расписание_Маршрут_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Расписание"
    ADD CONSTRAINT "Расписание_Маршрут_id_fkey" FOREIGN KEY ("Маршрут_id") REFERENCES public."Маршрут"(id) ON DELETE CASCADE;


--
-- Name: Расписание Расписание_Поезд_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Расписание"
    ADD CONSTRAINT "Расписание_Поезд_id_fkey" FOREIGN KEY ("Поезд_id") REFERENCES public."Поезд"(id) ON DELETE CASCADE;


--
-- Name: Расписание Расписание_Станция_отбытия_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Расписание"
    ADD CONSTRAINT "Расписание_Станция_отбытия_id_fkey" FOREIGN KEY ("Станция_отбытия_id") REFERENCES public."Станция"(id) ON DELETE CASCADE;


--
-- Name: Расписание Расписание_Станция_прибытия_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Расписание"
    ADD CONSTRAINT "Расписание_Станция_прибытия_id_fkey" FOREIGN KEY ("Станция_прибытия_id") REFERENCES public."Станция"(id) ON DELETE CASCADE;


--
-- Name: Сотрудник_Маршрут Сотрудник_Маршрут_Маршрут_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Сотрудник_Маршрут"
    ADD CONSTRAINT "Сотрудник_Маршрут_Маршрут_id_fkey" FOREIGN KEY ("Маршрут_id") REFERENCES public."Маршрут"(id) ON DELETE CASCADE;


--
-- Name: Сотрудник_Маршрут Сотрудник_Маршрут_Сотрудник_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Сотрудник_Маршрут"
    ADD CONSTRAINT "Сотрудник_Маршрут_Сотрудник_id_fkey" FOREIGN KEY ("Сотрудник_id") REFERENCES public."Сотрудник"(id) ON DELETE CASCADE;


--
-- PostgreSQL database dump complete
--

